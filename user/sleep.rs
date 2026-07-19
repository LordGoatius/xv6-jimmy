use core::ffi::CStr;

pub unsafe extern "C" {
    fn pause(u32) -> u32;
    fn exit(u32) -> !;
    fn printf(fmt: *const u8, ...) -> u32;
    fn atoi(str: &CStr) -> u32;
}

fn sleep(arg: &CStr) {
    let secs = ator(arg);
    pause(secs * TICKS_PER_SECOND);
}

extern "C" fn main(argc: u32, argv: *const &CStr) {
    if argc == 1 {
        printf("Please pass an argument");
        exit(1);
    } else if argc == 2 {
        sleep(unsafe {
            *argv.add(1)
        });
    }
    exit(0);
}
