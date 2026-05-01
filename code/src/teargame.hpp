/* SPDX-License-Identifier: GPL-2.0 */
/*
 * TearGame C++ User Space Library
 * 
 * 现代C++封装库，提供：
 * - 模板化读写操作
 * - RAII资源管理
 * - 批量操作支持
 * - 可选异常/错误码模式
 * 
 * 使用方法:
 *   #include <teargame.hpp>
 *   
 *   TearGame tear;
 *   tear.auth("MyApp");
 *   tear.attach("com.game.target");
 *   auto health = tear.read<int>(0x12345678);
 */

#ifndef _TEARGAME_HPP
#define _TEARGAME_HPP

#include <cstdint>
#include <cstring>
#include <string>
#include <vector>
#include <optional>
#include <memory>
#include <stdexcept>
#include <type_traits>
#include <functional>
#include <sys/prctl.h>
#include <unistd.h>
#include <time.h>

namespace teargame {

/*
 * ============================================================================
 * 常量定义
 * ============================================================================
 */
constexpr uint32_t MAGIC = 0x54454152;
constexpr uint32_t CMD_MAGIC = 0x19E5;
constexpr uint32_t CMD_AUTH = 0x1A2B;
constexpr uint32_t CMD_READ_MEMORY = 0x2E91;
constexpr uint32_t CMD_READ_SAFE = 0x3A82;
constexpr uint32_t CMD_READ_BATCH = 0x2EA0;
constexpr uint32_t CMD_READ_SCATTER = 0x2EB1;
constexpr uint32_t CMD_WRITE_MEMORY = 0x5C4D;
constexpr uint32_t CMD_WRITE_SAFE = 0x4B93;
constexpr uint32_t CMD_WRITE_BATCH = 0x5CA0;
constexpr uint32_t CMD_TOUCH_INIT = 0x4D8A;
constexpr uint32_t CMD_TOUCH_DOWN = 0x6F3B;
constexpr uint32_t CMD_TOUCH_MOVE = 0x7E2C;
constexpr uint32_t CMD_TOUCH_UP = 0x8D1F;
constexpr uint32_t CMD_GET_MODULE_BASE = 0x8B17;
constexpr uint32_t CMD_GET_MODULE_BSS = 0x3F72;
constexpr uint32_t CMD_FIND_PID = 0xB5E9;
constexpr uint32_t CMD_FIND_PID_STEALTH = 0xC7D3;
constexpr uint32_t CMD_FIND_PID_BYNAME = 0xF2C8;

constexpr size_t AUTH_KEY_LEN = 16;
constexpr uint32_t AUTH_HASH_SEED = 0x4A319941;
constexpr size_t BATCH_MAX_COUNT = 64;
constexpr size_t MAX_RW_SIZE = 1 << 20;  // 1MB

/*
 * ============================================================================
 * 内部结构体（与内核通信）
 * ============================================================================
 */
#pragma pack(push, 1)

struct CopyMemory {
    int32_t pid;
    int32_t reserved;
    uint64_t addr;
    uint64_t buffer;
    uint64_t size;
};

struct ModuleBase {
    int32_t pid;
    int32_t reserved;
    uint64_t name_ptr;
    uint64_t result;
};

struct GetPid {
    uint64_t name_ptr;
    int32_t result;
    int32_t reserved;
};

struct TouchInit {
    int32_t width;
    int32_t height;
};

struct TouchEvent {
    int32_t slot;
    int32_t x;
    int32_t y;
};

struct BatchItem {
    uint64_t addr;
    uint32_t size;
    int32_t result;
};

struct BatchRead {
    int32_t pid;
    uint32_t count;
    uint64_t items;
    uint64_t buffer;
    uint64_t buffer_size;
};

struct ScatterEntry {
    uint64_t addr;
    uint64_t buffer;
    uint32_t size;
    int32_t result;
};

struct ScatterRead {
    int32_t pid;
    uint32_t count;
    uint64_t entries;
};

#pragma pack(pop)

/*
 * ============================================================================
 * 错误处理
 * ============================================================================
 */
enum class Error {
    None = 0,
    NotLoaded = -1,
    NotAuthenticated = -2,
    NotAttached = -3,
    InvalidArgument = -4,
    ReadFailed = -5,
    WriteFailed = -6,
    ProcessNotFound = -7,
    ModuleNotFound = -8,
    TouchNotInitialized = -9,
    CopyFailed = -10,
    OutOfMemory = -11,
};

class TearException : public std::runtime_error {
public:
    Error error;
    
    explicit TearException(Error err, const std::string& msg = "")
        : std::runtime_error(msg.empty() ? errorToString(err) : msg), error(err) {}
    
    static std::string errorToString(Error err) {
        switch (err) {
            case Error::None: return "无错误";
            case Error::NotLoaded: return "模块未加载";
            case Error::NotAuthenticated: return "未认证";
            case Error::NotAttached: return "未附加进程";
            case Error::InvalidArgument: return "无效参数";
            case Error::ReadFailed: return "读取失败";
            case Error::WriteFailed: return "写入失败";
            case Error::ProcessNotFound: return "进程未找到";
            case Error::ModuleNotFound: return "模块未找到";
            case Error::TouchNotInitialized: return "触控未初始化";
            case Error::CopyFailed: return "复制失败";
            case Error::OutOfMemory: return "内存不足";
            default: return "未知错误";
        }
    }
};

/*
 * ============================================================================
 * 批量读取请求
 * ============================================================================
 */
struct ReadRequest {
    uint64_t addr;
    size_t size;
    void* buffer;  // 可选，如果为nullptr则自动分配
    
    ReadRequest(uint64_t a, size_t s, void* b = nullptr)
        : addr(a), size(s), buffer(b) {}
};

struct ReadResult {
    uint64_t addr;
    std::vector<uint8_t> data;
    bool success;
    int32_t error;
    
    template<typename T>
    T as() const {
        static_assert(std::is_trivially_copyable_v<T>, "T must be trivially copyable");
        if (data.size() < sizeof(T)) {
            return T{};
        }
        T result;
        std::memcpy(&result, data.data(), sizeof(T));
        return result;
    }
};

/*
 * ============================================================================
 * 内存区域封装
 * ============================================================================
 */
class MemoryRegion {
public:
    uint64_t base;
    size_t size;
    std::string name;
    
private:
    class TearGame* m_tear;
    
public:
    MemoryRegion(class TearGame* tear, uint64_t b, size_t s = 0, const std::string& n = "")
        : base(b), size(s), name(n), m_tear(tear) {}
    
    // 相对偏移读取
    template<typename T>
    T read(size_t offset) const;
    
    template<typename T>
    bool write(size_t offset, const T& value);
    
    // 读取字节数组
    std::vector<uint8_t> readBytes(size_t offset, size_t count) const;
    
    bool isValid() const { return base != 0; }
    operator bool() const { return isValid(); }
};

/*
 * ============================================================================
 * 触控设备封装
 * ============================================================================
 */
class TouchDevice {
private:
    bool m_initialized = false;
    int m_width = 0;
    int m_height = 0;
    
public:
    bool init(int width, int height) {
        TouchInit ti = {width, height};
        int ret = prctl(MAGIC, CMD_TOUCH_INIT, &ti, 0, 0);
        if (ret == 0) {
            m_initialized = true;
            m_width = width;
            m_height = height;
        }
        return ret == 0;
    }
    
    bool tap(int x, int y, int delayMs = 50) {
        if (!m_initialized) return false;
        
        TouchEvent te = {0, x, y};
        if (prctl(MAGIC, CMD_TOUCH_DOWN, &te, 0, 0) != 0)
            return false;
        
        if (delayMs > 0) {
            usleep(delayMs * 1000);
        }
        
        return prctl(MAGIC, CMD_TOUCH_UP, &te, 0, 0) == 0;
    }
    
    bool swipe(int x1, int y1, int x2, int y2, int steps = 20, int durationMs = 500) {
        if (!m_initialized) return false;
        
        int dx = (x2 - x1) / steps;
        int dy = (y2 - y1) / steps;
        int stepDelay = durationMs * 1000 / steps;
        
        TouchEvent te = {0, x1, y1};
        if (prctl(MAGIC, CMD_TOUCH_DOWN, &te, 0, 0) != 0)
            return false;
        
        for (int i = 1; i <= steps; i++) {
            usleep(stepDelay);
            te.x = x1 + dx * i;
            te.y = y1 + dy * i;
            prctl(MAGIC, CMD_TOUCH_MOVE, &te, 0, 0);
        }
        
        te.x = x2;
        te.y = y2;
        prctl(MAGIC, CMD_TOUCH_MOVE, &te, 0, 0);
        usleep(stepDelay);
        
        return prctl(MAGIC, CMD_TOUCH_UP, &te, 0, 0) == 0;
    }
    
    bool down(int slot, int x, int y) {
        if (!m_initialized) return false;
        TouchEvent te = {slot, x, y};
        return prctl(MAGIC, CMD_TOUCH_DOWN, &te, 0, 0) == 0;
    }
    
    bool move(int slot, int x, int y) {
        if (!m_initialized) return false;
        TouchEvent te = {slot, x, y};
        return prctl(MAGIC, CMD_TOUCH_MOVE, &te, 0, 0) == 0;
    }
    
    bool up(int slot) {
        if (!m_initialized) return false;
        TouchEvent te = {slot, 0, 0};
        return prctl(MAGIC, CMD_TOUCH_UP, &te, 0, 0) == 0;
    }
    
    bool isInitialized() const { return m_initialized; }
    int width() const { return m_width; }
    int height() const { return m_height; }
};

/*
 * ============================================================================
 * 主类 TearGame
 * ============================================================================
 */
class TearGame {
private:
    int m_pid = 0;
    bool m_authed = false;
    bool m_throwExceptions = false;
    Error m_lastError = Error::None;
    TouchDevice m_touch;
    
    // 生成认证密钥
    static std::string generateKey(const std::string& magic, long timestamp) {
        int hash = static_cast<int>(timestamp * AUTH_HASH_SEED) - 0x7ACE8F1A;
        
        size_t magicLen = std::min(magic.length(), size_t(128));
        for (size_t i = 0; i < magicLen; i++) {
            hash = hash * 31 + static_cast<unsigned char>(magic[i]);
        }
        
        std::string key(AUTH_KEY_LEN, '\0');
        for (size_t i = 0; i < AUTH_KEY_LEN; i++) {
            int val = i + (hash >> (i & 7));
            int absVal = (val < 0) ? -val : val;
            key[i] = 'A' + (absVal % 26);
            hash = i + hash * 17;
        }
        
        return key;
    }

    static uint32_t readToken() {
        uint64_t slot = static_cast<uint64_t>(time(nullptr)) / 30ULL;
        uint32_t x = static_cast<uint32_t>(slot);

        x ^= MAGIC;
        x ^= static_cast<uint32_t>(getpid()) * 0x9E3779B9U;
        x ^= AUTH_HASH_SEED;
        x ^= x >> 16;
        x *= 0x7FEB352DU;
        x ^= x >> 15;
        x *= 0x846CA68BU;
        x ^= x >> 16;
        return x ? x : 0xA5A5A5A5U;
    }
    
    void setError(Error err) {
        m_lastError = err;
        if (m_throwExceptions && err != Error::None) {
            throw TearException(err);
        }
    }

public:
    TearGame() = default;
    
    // 启用/禁用异常模式
    void setThrowExceptions(bool enable) { m_throwExceptions = enable; }
    bool throwExceptions() const { return m_throwExceptions; }
    
    // 获取最后错误
    Error lastError() const { return m_lastError; }
    std::string lastErrorString() const { return TearException::errorToString(m_lastError); }
    
    // 检查模块是否加载
    bool isLoaded() {
        long result = prctl(MAGIC, CMD_MAGIC, 0, 0, 0);
        return result == static_cast<long>(MAGIC);
    }
    
    // 认证
    bool auth(const std::string& magic) {
        if (!isLoaded()) {
            setError(Error::NotLoaded);
            return false;
        }
        
        time_t now = time(nullptr);
        std::string key = generateKey(magic, now);
        std::string keyStr = magic + ":" + std::to_string(now) + ":" + key;
        
        int ret = prctl(MAGIC, CMD_AUTH, keyStr.c_str(), 0, 0);
        m_authed = (ret == 0);
        
        if (!m_authed) {
            setError(Error::NotAuthenticated);
        } else {
            m_lastError = Error::None;
        }
        
        return m_authed;
    }
    
    bool isAuthenticated() const { return m_authed; }
    
    // 附加进程
    bool attach(int pid) {
        if (!m_authed) {
            setError(Error::NotAuthenticated);
            return false;
        }
        m_pid = pid;
        m_lastError = Error::None;
        return true;
    }
    
    bool attach(const std::string& name) {
        if (!m_authed) {
            setError(Error::NotAuthenticated);
            return false;
        }
        
        int pid = findPid(name);
        if (pid <= 0) {
            setError(Error::ProcessNotFound);
            return false;
        }
        
        m_pid = pid;
        m_lastError = Error::None;
        return true;
    }
    
    bool isAttached() const { return m_pid > 0; }
    int pid() const { return m_pid; }
    
    // 查找进程
    int findPid(const std::string& name) {
        GetPid gp = {};
        gp.name_ptr = reinterpret_cast<uint64_t>(name.c_str());
        
        if (prctl(MAGIC, CMD_FIND_PID, &gp, 0, 0) == 0) {
            return gp.result;
        }
        return 0;
    }
    
    int findPidStealth(const std::string& name) {
        GetPid gp = {};
        gp.name_ptr = reinterpret_cast<uint64_t>(name.c_str());
        
        if (prctl(MAGIC, CMD_FIND_PID_STEALTH, &gp, 0, 0) == 0) {
            return gp.result;
        }
        return 0;
    }
    
    // 模板化读取
    template<typename T>
    T read(uint64_t addr) {
        static_assert(std::is_trivially_copyable_v<T>, "T must be trivially copyable");
        
        if (!isAttached()) {
            setError(Error::NotAttached);
            return T{};
        }
        
        T result{};
        CopyMemory cm = {};
        cm.pid = m_pid;
        cm.addr = addr;
        cm.buffer = reinterpret_cast<uint64_t>(&result);
        cm.size = sizeof(T);
        
        if (prctl(MAGIC, CMD_READ_MEMORY, &cm, readToken(), 0) != 0) {
            setError(Error::ReadFailed);
            return T{};
        }
        
        m_lastError = Error::None;
        return result;
    }
    
    // 安全读取（使用内核的安全拷贝）
    template<typename T>
    T readSafe(uint64_t addr) {
        static_assert(std::is_trivially_copyable_v<T>, "T must be trivially copyable");
        
        if (!isAttached()) {
            setError(Error::NotAttached);
            return T{};
        }
        
        T result{};
        CopyMemory cm = {};
        cm.pid = m_pid;
        cm.addr = addr;
        cm.buffer = reinterpret_cast<uint64_t>(&result);
        cm.size = sizeof(T);
        
        if (prctl(MAGIC, CMD_READ_SAFE, &cm, readToken(), 0) != 0) {
            setError(Error::ReadFailed);
            return T{};
        }
        
        m_lastError = Error::None;
        return result;
    }
    
    // 读取到缓冲区
    bool readBuffer(uint64_t addr, void* buffer, size_t size) {
        if (!isAttached()) {
            setError(Error::NotAttached);
            return false;
        }
        
        CopyMemory cm = {};
        cm.pid = m_pid;
        cm.addr = addr;
        cm.buffer = reinterpret_cast<uint64_t>(buffer);
        cm.size = size;
        
        if (prctl(MAGIC, CMD_READ_MEMORY, &cm, readToken(), 0) != 0) {
            setError(Error::ReadFailed);
            return false;
        }
        
        m_lastError = Error::None;
        return true;
    }
    
    // 读取字节数组
    std::vector<uint8_t> readBytes(uint64_t addr, size_t size) {
        std::vector<uint8_t> result(size);
        if (!readBuffer(addr, result.data(), size)) {
            return {};
        }
        return result;
    }
    
    // 读取字符串
    std::string readString(uint64_t addr, size_t maxLen = 256) {
        auto bytes = readBytes(addr, maxLen);
        if (bytes.empty()) return "";
        
        auto nullPos = std::find(bytes.begin(), bytes.end(), 0);
        return std::string(bytes.begin(), nullPos);
    }
    
    // 批量读取
    std::vector<ReadResult> readBatch(const std::vector<ReadRequest>& requests) {
        std::vector<ReadResult> results;
        results.reserve(requests.size());
        
        if (!isAttached() || requests.empty()) {
            setError(Error::NotAttached);
            return results;
        }
        
        // 计算总大小
        size_t totalSize = 0;
        for (const auto& req : requests) {
            totalSize += req.size;
        }
        
        // 分配缓冲区
        std::vector<uint8_t> buffer(totalSize);
        std::vector<BatchItem> items(requests.size());
        
        for (size_t i = 0; i < requests.size(); i++) {
            items[i].addr = requests[i].addr;
            items[i].size = static_cast<uint32_t>(requests[i].size);
            items[i].result = 0;
        }
        
        // 执行批量读取
        BatchRead br = {};
        br.pid = m_pid;
        br.count = static_cast<uint32_t>(requests.size());
        br.items = reinterpret_cast<uint64_t>(items.data());
        br.buffer = reinterpret_cast<uint64_t>(buffer.data());
        br.buffer_size = totalSize;
        
        prctl(MAGIC, CMD_READ_BATCH, &br, readToken(), 0);
        
        // 处理结果
        size_t offset = 0;
        for (size_t i = 0; i < requests.size(); i++) {
            ReadResult r;
            r.addr = requests[i].addr;
            r.error = items[i].result;
            r.success = (items[i].result >= 0);
            
            if (r.success && items[i].result > 0) {
                r.data.assign(buffer.begin() + offset, 
                              buffer.begin() + offset + items[i].result);
                offset += items[i].result;
            }
            
            results.push_back(std::move(r));
        }
        
        m_lastError = Error::None;
        return results;
    }
    
    // 分散读取
    std::vector<ReadResult> readScatter(const std::vector<ReadRequest>& requests) {
        std::vector<ReadResult> results;
        results.reserve(requests.size());
        
        if (!isAttached() || requests.empty()) {
            setError(Error::NotAttached);
            return results;
        }
        
        std::vector<ScatterEntry> entries(requests.size());
        std::vector<std::vector<uint8_t>> buffers(requests.size());
        
        for (size_t i = 0; i < requests.size(); i++) {
            buffers[i].resize(requests[i].size);
            entries[i].addr = requests[i].addr;
            entries[i].buffer = reinterpret_cast<uint64_t>(buffers[i].data());
            entries[i].size = static_cast<uint32_t>(requests[i].size);
            entries[i].result = 0;
        }
        
        ScatterRead sr = {};
        sr.pid = m_pid;
        sr.count = static_cast<uint32_t>(requests.size());
        sr.entries = reinterpret_cast<uint64_t>(entries.data());
        
        prctl(MAGIC, CMD_READ_SCATTER, &sr, readToken(), 0);
        
        for (size_t i = 0; i < requests.size(); i++) {
            ReadResult r;
            r.addr = requests[i].addr;
            r.error = entries[i].result;
            r.success = (entries[i].result >= 0);
            
            if (r.success) {
                r.data = std::move(buffers[i]);
            }
            
            results.push_back(std::move(r));
        }
        
        m_lastError = Error::None;
        return results;
    }
    
    // 模板化写入
    template<typename T>
    bool write(uint64_t addr, const T& value) {
        static_assert(std::is_trivially_copyable_v<T>, "T must be trivially copyable");
        
        if (!isAttached()) {
            setError(Error::NotAttached);
            return false;
        }
        
        CopyMemory cm = {};
        cm.pid = m_pid;
        cm.addr = addr;
        cm.buffer = reinterpret_cast<uint64_t>(&value);
        cm.size = sizeof(T);
        
        if (prctl(MAGIC, CMD_WRITE_MEMORY, &cm, 0, 0) != 0) {
            setError(Error::WriteFailed);
            return false;
        }
        
        m_lastError = Error::None;
        return true;
    }
    
    // 安全写入
    template<typename T>
    bool writeSafe(uint64_t addr, const T& value) {
        static_assert(std::is_trivially_copyable_v<T>, "T must be trivially copyable");
        
        if (!isAttached()) {
            setError(Error::NotAttached);
            return false;
        }
        
        CopyMemory cm = {};
        cm.pid = m_pid;
        cm.addr = addr;
        cm.buffer = reinterpret_cast<uint64_t>(&value);
        cm.size = sizeof(T);
        
        if (prctl(MAGIC, CMD_WRITE_SAFE, &cm, 0, 0) != 0) {
            setError(Error::WriteFailed);
            return false;
        }
        
        m_lastError = Error::None;
        return true;
    }
    
    // 写入缓冲区
    bool writeBuffer(uint64_t addr, const void* buffer, size_t size) {
        if (!isAttached()) {
            setError(Error::NotAttached);
            return false;
        }
        
        CopyMemory cm = {};
        cm.pid = m_pid;
        cm.addr = addr;
        cm.buffer = reinterpret_cast<uint64_t>(buffer);
        cm.size = size;
        
        if (prctl(MAGIC, CMD_WRITE_MEMORY, &cm, 0, 0) != 0) {
            setError(Error::WriteFailed);
            return false;
        }
        
        m_lastError = Error::None;
        return true;
    }
    
    // 获取模块基址
    uint64_t getModuleBase(const std::string& name) {
        if (!isAttached()) {
            setError(Error::NotAttached);
            return 0;
        }
        
        ModuleBase mb = {};
        mb.pid = m_pid;
        mb.name_ptr = reinterpret_cast<uint64_t>(name.c_str());
        
        if (prctl(MAGIC, CMD_GET_MODULE_BASE, &mb, 0, 0) == 0) {
            m_lastError = Error::None;
            return mb.result;
        }
        
        setError(Error::ModuleNotFound);
        return 0;
    }
    
    // 获取模块BSS地址
    uint64_t getModuleBss(const std::string& name) {
        if (!isAttached()) {
            setError(Error::NotAttached);
            return 0;
        }
        
        ModuleBase mb = {};
        mb.pid = m_pid;
        mb.name_ptr = reinterpret_cast<uint64_t>(name.c_str());
        
        if (prctl(MAGIC, CMD_GET_MODULE_BSS, &mb, 0, 0) == 0) {
            m_lastError = Error::None;
            return mb.result;
        }
        
        setError(Error::ModuleNotFound);
        return 0;
    }
    
    // 获取模块区域
    MemoryRegion module(const std::string& name) {
        uint64_t base = getModuleBase(name);
        return MemoryRegion(this, base, 0, name);
    }
    
    // 获取触控设备
    TouchDevice& touch() { return m_touch; }
};

/*
 * ============================================================================
 * MemoryRegion 方法实现
 * ============================================================================
 */
template<typename T>
T MemoryRegion::read(size_t offset) const {
    if (!m_tear || !isValid()) return T{};
    return m_tear->read<T>(base + offset);
}

template<typename T>
bool MemoryRegion::write(size_t offset, const T& value) {
    if (!m_tear || !isValid()) return false;
    return m_tear->write<T>(base + offset, value);
}

inline std::vector<uint8_t> MemoryRegion::readBytes(size_t offset, size_t count) const {
    if (!m_tear || !isValid()) return {};
    return m_tear->readBytes(base + offset, count);
}

/*
 * ============================================================================
 * 便捷函数
 * ============================================================================
 */

// 快速创建并认证
inline std::unique_ptr<TearGame> make_teargame(const std::string& magic) {
    auto tear = std::make_unique<TearGame>();
    if (!tear->auth(magic)) {
        return nullptr;
    }
    return tear;
}

// 快速创建、认证并附加
inline std::unique_ptr<TearGame> make_teargame(const std::string& magic, 
                                               const std::string& processName) {
    auto tear = make_teargame(magic);
    if (!tear) return nullptr;
    
    if (!tear->attach(processName)) {
        return nullptr;
    }
    return tear;
}

} // namespace teargame

// 提供全局别名方便使用
using TearGame = teargame::TearGame;
using TearError = teargame::Error;
using TearException = teargame::TearException;
using TearReadRequest = teargame::ReadRequest;
using TearReadResult = teargame::ReadResult;
using TearMemoryRegion = teargame::MemoryRegion;
using TearTouchDevice = teargame::TouchDevice;

#endif /* _TEARGAME_HPP */
