#include <linux/module.h>
#define INCLUDE_VERMAGIC
#include <linux/build-salt.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

BUILD_SALT;

MODULE_INFO(vermagic, VERMAGIC_STRING);
MODULE_INFO(name, KBUILD_MODNAME);

__visible struct module __this_module
__section(".gnu.linkonce.this_module") = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

#ifdef CONFIG_RETPOLINE
MODULE_INFO(retpoline, "Y");
#endif

static const struct modversion_info ____versions[]
__used __section("__versions") = {
	{ 0x7c24b32d, "module_layout" },
	{ 0x148653, "vsnprintf" },
	{ 0x656e4a6e, "snprintf" },
	{ 0xc2a656d9, "__get_task_comm" },
	{ 0x7a2af7b4, "cpu_number" },
	{ 0xb43f9365, "ktime_get" },
	{ 0x5a9f1d63, "memmove" },
	{ 0x65671f08, "param_ops_bool" },
	{ 0xaf56600a, "arm64_use_ng_mappings" },
	{ 0xb5b54b34, "_raw_spin_unlock" },
	{ 0xba8fbd64, "_raw_spin_lock" },
	{ 0x2e72ceb5, "d_path" },
	{ 0x779a18af, "kstrtoll" },
	{ 0x349cba85, "strchr" },
	{ 0xc310b981, "strnstr" },
	{ 0x5243bc7e, "debugfs_remove" },
	{ 0x299f72da, "debugfs_lookup" },
	{ 0x18b23dc5, "unregister_kprobe" },
	{ 0xc502eb6d, "register_kprobe" },
	{ 0x4829a47e, "memcpy" },
	{ 0xe1537255, "__list_del_entry_valid" },
	{ 0xf524bddc, "kobject_del" },
	{ 0x36505ade, "try_module_get" },
	{ 0x2f166344, "module_put" },
	{ 0xf5f7b498, "kobject_add" },
	{ 0x68f31cbd, "__list_add_valid" },
	{ 0x8ea54251, "unregister_kretprobe" },
	{ 0x881262d4, "register_kretprobe" },
	{ 0xd36dc10c, "get_random_u32" },
	{ 0x78c7940d, "mutex_trylock" },
	{ 0xdd4d55b6, "_raw_read_unlock" },
	{ 0xeb078aee, "_raw_write_unlock_irqrestore" },
	{ 0xb2ef0fff, "input_mt_sync_frame" },
	{ 0x3fcaa46c, "input_mt_report_slot_state" },
	{ 0x3b938c8d, "input_event" },
	{ 0x5021bd81, "_raw_write_lock_irqsave" },
	{ 0x889b1370, "_raw_read_trylock" },
	{ 0xd329e1c6, "input_free_device" },
	{ 0xff775d7a, "input_register_device" },
	{ 0x8bdf6c84, "input_mt_init_slots" },
	{ 0x837ac1f4, "input_set_abs_params" },
	{ 0xe3db3c44, "input_allocate_device" },
	{ 0x715a5ed0, "vprintk" },
	{ 0xe8b268ae, "mutex_unlock" },
	{ 0xdf5b2f63, "input_unregister_device" },
	{ 0xeb9065d9, "mutex_lock" },
	{ 0x574add77, "__mutex_init" },
	{ 0x2f92131, "find_task_by_vpid" },
	{ 0x9166fada, "strncpy" },
	{ 0xb38391e9, "kmem_cache_alloc_trace" },
	{ 0x8900b200, "kmalloc_caches" },
	{ 0x6091797f, "synchronize_rcu" },
	{ 0xd35cce70, "_raw_spin_unlock_irqrestore" },
	{ 0x28aa6a67, "call_rcu" },
	{ 0x34db050b, "_raw_spin_lock_irqsave" },
	{ 0x88f5cdef, "down_read_trylock" },
	{ 0x1e6d26a8, "strstr" },
	{ 0xe2d5255a, "strcmp" },
	{ 0x5a921311, "strncmp" },
	{ 0xb1307de2, "init_task" },
	{ 0x98cf60b3, "strlen" },
	{ 0x6c1d25ab, "access_process_vm" },
	{ 0x918779bc, "pid_task" },
	{ 0x12aec6f2, "find_vpid" },
	{ 0x296695f, "refcount_warn_saturate" },
	{ 0x825a3481, "mmput" },
	{ 0x43b0c9c3, "preempt_schedule" },
	{ 0x6b50e951, "up_read" },
	{ 0x806ab898, "find_vma" },
	{ 0x3355da1c, "down_read" },
	{ 0x9a80f9a6, "__put_task_struct" },
	{ 0xa378a2f4, "get_task_mm" },
	{ 0xd3c5d4d0, "put_pid" },
	{ 0xd15a2984, "get_pid_task" },
	{ 0xdca62649, "find_get_pid" },
	{ 0x51e77c97, "pfn_valid" },
	{ 0x2469810f, "__rcu_read_unlock" },
	{ 0x8d522714, "__rcu_read_lock" },
	{ 0xc9ec4e21, "free_percpu" },
	{ 0x949f7342, "__alloc_percpu" },
	{ 0x9688de8b, "memstart_addr" },
	{ 0x280f9f14, "__per_cpu_offset" },
	{ 0x17de3d5, "nr_cpu_ids" },
	{ 0x92ad1db9, "cpumask_next" },
	{ 0xde7ece30, "__cpu_possible_mask" },
	{ 0x6b2941b2, "__arch_copy_to_user" },
	{ 0x8b9f70c7, "cpu_hwcaps" },
	{ 0xaf507de1, "__arch_copy_from_user" },
	{ 0x4b0a3f52, "gic_nonsecure_priorities" },
	{ 0xec2fc692, "cpu_hwcap_keys" },
	{ 0x14b89635, "arm64_const_caps_ready" },
	{ 0xdcb764ad, "memset" },
	{ 0x24428be5, "strncpy_from_user" },
	{ 0x599fb41c, "kvmalloc_node" },
	{ 0x37a0cba, "kfree" },
	{ 0x7aa1756e, "kvfree" },
	{ 0x88db9f48, "__check_object_size" },
	{ 0xeb233a45, "__kmalloc" },
	{ 0x98a9d10c, "__stack_chk_fail" },
	{ 0x15ba50a6, "jiffies" },
	{ 0x9ec6ca96, "ktime_get_real_ts64" },
	{ 0x8f678b07, "__stack_chk_guard" },
	{ 0xc5850110, "printk" },
};

MODULE_INFO(depends, "");


MODULE_INFO(srcversion, "81BC50CB2AAD8988E609B94");
