#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

MODULE_INFO(vermagic, VERMAGIC_STRING);

struct module __this_module
__attribute__((section(".gnu.linkonce.this_module"))) = {
 .name = KBUILD_MODNAME,
 .init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
 .exit = cleanup_module,
#endif
};

static const struct modversion_info ____versions[]
__attribute_used__
__attribute__((section("__versions"))) = {
	{ 0x89e24b9c, "struct_module" },
	{ 0x59968f3c, "__wake_up" },
	{ 0xbd4c9cf0, "module_refcount" },
	{ 0x72270e35, "do_gettimeofday" },
	{ 0x4a8db44f, "univ_read_intvector" },
	{ 0x81c68817, "univ_dma_read" },
	{ 0xb0077fa0, "univ_vread32" },
	{ 0xf4cf00e9, "univ_vread16" },
	{ 0xc437a7d7, "univ_vwrite16" },
	{ 0x23586c79, "univ_set_amsr" },
	{ 0x2da418b5, "copy_to_user" },
	{ 0x8fd3522b, "univ_define_intlevel" },
	{ 0x2e60bace, "memcpy" },
	{ 0x693040e1, "univ_map_write16" },
	{ 0xc0f45823, "univ_map_write32" },
	{ 0xe32b91aa, "univ_map_read32" },
	{ 0x26e96637, "request_irq" },
	{ 0x19cacd0, "init_waitqueue_head" },
	{ 0x4086729e, "register_chrdev" },
	{ 0xd08c3dea, "univ_init_dma" },
	{ 0xd9aa794d, "univ_init_window" },
	{ 0xfd77a4ac, "univ_map_read16" },
	{ 0xe5541f0a, "univ_end_dma" },
	{ 0x244078c1, "univ_end_window" },
	{ 0x1b7d4074, "printk" },
	{ 0xc192d491, "unregister_chrdev" },
	{ 0xf20dabd8, "free_irq" },
};

static const char __module_depends[]
__attribute_used__
__attribute__((section(".modinfo"))) =
"depends=bb-sva041";


MODULE_INFO(srcversion, "6B152C0246C33A191EB10E4");
