#![feature(lang_items)]
#![no_std]

#[no_mangle]
pub extern fn tupai_main()
{
	return;
}

#[lang = "eh_personality"]
extern fn eh_personality() {} // Unwinding stub

#[lang = "panic_fmt"]
#[no_mangle]
pub extern fn panic_fmt() -> ! // Panic entry
{
	loop {}
}
