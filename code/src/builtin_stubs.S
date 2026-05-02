/* SPDX-License-Identifier: GPL-2.0 */
/*
 * Compiler builtin stubs for Clang on ARM64.
 * Clang generates __memcpy/__memmove/__memset which the kernel
 * does not export. Redirect to kernel's standard implementations.
 */

.section .text.__memcpy, "ax", %progbits
.globl __memcpy
.type __memcpy, %function
__memcpy:
    b memcpy
.size __memcpy, .-__memcpy

.section .text.__memmove, "ax", %progbits
.globl __memmove
.type __memmove, %function
__memmove:
    b memmove
.size __memmove, .-__memmove

.section .text.__memset, "ax", %progbits
.globl __memset
.type __memset, %function
__memset:
    b memset
.size __memset, .-__memset
