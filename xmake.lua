--
-- > Notice: Amélie Heinrich @ 2025
-- > Create Time: 2025-03-23 15:06:40
--

add_rules("mode.debug", "mode.release")

target("AngryBirds")
    set_languages("c99")
    set_rundir(".")

    add_files("src/*.c")
    add_linkdirs("ext/lib")
    add_includedirs("ext/include")
    add_syslinks("raylib", "gdi32", "msvcrt", "winmm", "user32", "shell32",
                 "/NODEFAULTLIB:libcmt") -- Absolute tomfoolery, even the biggest sigma skibidi rizzler wouldn't have thought of this GENIUS move.
                                         -- I am literally Kira from Death Note and you cannot stop my wrath
    