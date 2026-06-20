add_rules("mode.debug", "mode.release", "mode.releasedbg")

add_repositories("liteldev-repo https://github.com/LiteLDev/xmake-repo.git")
add_repositories("groupmountain-repo https://github.com/GroupMountain/xmake-repo.git")

if not has_config("vs_runtime") then
    set_runtimes("MD")
end

add_requires("levilamina 26.10.5",{configs = {target_type = "server"}})
add_requires("levibuildscript")
add_requires("gmlib 26.10.0")
add_requires("modapi 26.10.0")

target("DeathMessages") -- Change this to your mod name.
    add_rules("@levibuildscript/linkrule")
    add_rules("@levibuildscript/modpacker")
    add_cxflags( "/EHa", "/utf-8", "/W4", "/w44265", "/w44289", "/w44296", "/w45263", "/w44738", "/w45204")
    add_defines("NOMINMAX", "UNICODE")
    add_packages("levilamina","gmlib","modapi")
    set_exceptions("none") -- To avoid conflicts with /EHa.
    set_kind("shared")
    set_languages("c++23")
    set_symbols("debug")
    add_headerfiles("src/**.h")
    add_files("src/**.cpp")
    add_includedirs("src")
