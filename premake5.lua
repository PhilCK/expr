-- [ Build script ] --
--[[
    The build system id data orientated. It works by having premake.json files
    in repo, then all the files are parsed here, then finally the parsed data
    is translated into premake calls. No premake api interaction should happen
    outside of this script.
]]--


--------------------------------------------------------------------- Globals --


out_dir = "./build"
solution_name = "Expression"
debug_print = false -- if set to true, dumps out alot of dialog to inspect --
default_lang = "C-ANSI"

compiler = "msvs"

if os.target() == "linux" then
    compiler = "gcc"
elseif os.target() == "macosx" then
    compiler = "clang"
end


--------------------------------------------------------------------- Helpers --


function
find_table(proj, ident)

    local results = {}

    local tags = {os.target(), compiler}

    -- Find entries that start with ident --
    for k, v in pairs(proj) do

        -- Check all tags match our tags --
        if string.startswith(k, ident) then
            parts = string.explode(k, "-")

            local contains_all = true;

            for i, j in pairs(parts) do

                if not table.contains(tags, j) and i > 1 then
                    contains_all = false;
                end
            end

            if contains_all then
                table.insert(results, v);
            end
        end
    end


    return table.flatten(results)

    -- local results = {}

    -- local data = proj[ident]
    -- if data then results = table.join(results, data) end

    -- local plat_data = proj[ident .. "_" .. os.target()]
    -- if plat_data then results = table.join(results, plat_data) end

    -- return results

end


---------------------------------------------------------- Find Project Files --
--[[
    Finds all the premake.json files and gets them ready for processing.
    Add any missing tables so we don't have to keep checking for their
    existance.
]]--


project_files = os.matchfiles("./**premake.json")

print("searching for premake.json")

local projects = {}

for i, file in ipairs(project_files) do

    local json_str = io.readfile(file)
    local file_table, err = json.decode(json_str)

    -- Some Checks --
    if err then
        print("Failed to read file " .. file .. " with err " .. err)
        goto continue
    end 
    
    if not file_table then
        print("Failed to read table in " .. file)
        goto continue
    end

    -- file can have multiple tables --
    for j, proj_table in ipairs(file_table.projects) do

        -- Update file paths --
        local files = find_table(proj_table, "files")

        for k, f in ipairs(files) do

            local abs_dir   = path.getabsolute(file);
            local dir       = path.getdirectory(abs_dir)
            local new_path  = dir .. "/".. f

            files[k] = new_path

        end

        proj_table.files = files;

        -- Update inc paths --
        if proj_table.include_dirs_public then
            for k, f in ipairs(proj_table.include_dirs_public) do
            
                local abs_dir   = path.getabsolute(file)
                local dir       = path.getdirectory(abs_dir)
                local new_path  = dir .. "/" .. f

                proj_table.include_dirs_public[k] = new_path
            
            end
        end

        -- Update inc paths --
        if proj_table.include_dirs then
            for k, f in ipairs(proj_table.include_dirs) do
    
                local abs_dir   = path.getabsolute(file)
                local dir       = path.getdirectory(abs_dir)
                local new_path  = dir .. "/" .. f

                proj_table.include_dirs[k] = new_path

            end
        end 

        -- Linking --
        if proj_table.linkable == nil then
            proj_table["linkable"] = true;
        end

        -- Add missing tables so we don't have to check everything --
        -- This is superseded by find_table --
        local tables = {
            "files", "defines", "links" , "linkoptions", "library_dirs",
            "dependencies", "include_dirs", "buildoptions"
        }

        for k, f in ipairs(tables) do
            if not proj_table[f] then
                proj_table[f] = {}
            end
        end

        -- Add to list --
        table.insert(projects, proj_table)
    end

    ::continue::
end


----------------------------- Transform projects table into premake api calls --


print("preprocessing project information")


-------------------------------------------------------- Include Dependencies --
--[[
    This will pull in only its direct dependencies public include dirs
]]--


function
inc_search(root, curr_proj, projects)

    local deps = find_table(curr_proj, "dependencies")

    for i, dep_name in ipairs(deps) do
        
        for j, dep_proj in ipairs(projects) do

            if(dep_proj.name == dep_name) then
                inc_search(root, dep_proj, projects)

                local inc_dirs = find_table(dep_proj, "include_dirs_public")

                table.insert(
                    root.include_dirs,
                    inc_dirs)
            end
        end
    end

end


for i, proj in ipairs(projects) do
  
    -- Merge public dirs for this project only --
    table.insert(
        proj.include_dirs,
        find_table(proj, "include_dirs_public"))

    -- Include dirs --
    
    inc_search(proj, proj, projects)
    table.flatten(proj.include_dirs)

    -- for j, dep in ipairs(deps) do

    --     for k, dep_proj in ipairs(projects) do

    --         if(dep_proj.name == dep) then
    --             table.insert(
    --                 proj.include_dirs,
    --                 find_table(dep_proj, "include_dirs_public"))
                
    --             proj.include_dirs = table.flatten(proj.include_dirs)
    --         end
    --     end
    -- end

end


-------------------------------------------------------- Library Dependencies --
--[[
    For projects that are executable, this will recusivily check dependencies
    for libs, and lib dirs
]]--


function
dep_search(root, curr_proj, projects)

    for i, dep_name in ipairs(curr_proj.dependencies) do
        
        for j, dep_proj in ipairs(projects) do

            if(dep_proj.name == dep_name) then
                dep_search(root, dep_proj, projects)

                if dep_proj.linkable == true then
                    table.insert(root.links, dep_name)
                end
            end
        end
         
    end

end


-- function
-- lib_search(root, curr_proj, projects)

--     for i, dep_name in ipairs(curr_proj.dependencies) do
        
--         for j, dep_proj in ipairs(projects) do

--             if dep_proj.name == dep_name then
--                 lib_search(root, dep_proj, projects)

--                 if dep_proj.linkable == true then
--                     table.insert(root.links, dep_proj.name)
--                 end
--             end
--         end
         
--     end

-- end


for i, proj in ipairs(projects) do

    -- Skip unless this is an executable --
    if proj["kind"] ~= "ConsoleApp" and proj["kind"] ~= "WindowedApp" then
        goto continue
    end

    -- Recursive search --
    dep_search(proj, proj, projects)
    -- lib_search(proj, proj, projects)

    ::continue::

end


-------------------------------------------------------------------- Language --
--[[
    MSVS handles language standard quite differently to other compiles.
]]--


for i, proj in ipairs(projects) do
 
    local lang_table = {
        -- C Flavours --
        ["C-ANSI"] = { ["lang"] = "C", ["gnu"] = "-std=c89 -ansi",  },
        ["C"]      = { ["lang"] = "C", ["gnu"] = "-std=gnu89",      },
        ["C89"]    = { ["lang"] = "C", ["gnu"] = "-std=gnu89",      },
        ["C99"]    = { ["lang"] = "C", ["gnu"] = "-std=c99",        },
        ["C11"]    = { ["lang"] = "C", ["gnu"] = "-std=c11",        },

        -- C++ Flavours --
        ["C++"]   = { ["lang"] = "C++", ["gnu"] = "-std=c++14", },
        ["C++11"] = { ["lang"] = "C++", ["gnu"] = "-std=c++11", },
        ["C++14"] = { ["lang"] = "C++", ["gnu"] = "-std=c++14", },
        ["C++17"] = { ["lang"] = "C++", ["gnu"] = "-std=c++17", },
        ["C++20"] = { ["lang"] = "C++", ["gnu"] = "-std=c++20", },
    }

    -- Default is C -- 
    if not proj.language then
        proj.language = default_lang
    end

    if lang_table[proj.language] then
        if os.target() == "macosx" or os.target() == "linux" then
            table.insert(
                proj.buildoptions,
                lang_table[proj.language]["gnu"]);
        end

        proj.language = lang_table[proj.language]["lang"];
    end

end


-------------------------------------------- Generic / Platform Premake Setup --


solution(solution_name)
location(out_dir)

if compiler == "msvs" then
    characterset("MBCS")
end

if os.target() == "windows" then
    defines({"NOMINMAX", "WIN32_LEAN_AND_MEAN", "_CRT_SECURE_NO_WARNINGS"})
end

if os.target() == "macosx" then
    xcodebuildsettings({['ALWAYS_SEARCH_USER_PATHS'] = 'YES'})
end


--------------------------------------------------------------------- Configs --


local configs = {
    {
        name = "Development",
        symbols = true,
        simd = false,
        fast_float = true,
        extra_warnings = true,
        warnings_as_errors = false,
    },
    {
        name = "Staging",
        symbols = true,
        simd = true,
        optimize = true,
        fast_float = true,
        extra_warnings = true,
        warnings_as_errors = true,
    },
    {
        name = "Release",
        symbols = false,
        simd = true,
        optimize = true,
        fast_float = true,
        extra_warnings = true,
        warnings_as_errors = true,

        defines = {"NDEBUG"}
    },
}

local config_names = {}

for i, config in ipairs(configs) do
        table.insert(config_names, config.name)
end

configurations(config_names);


-------------------------------------------------------------------- Projects --


-- Loop through projects and geneate API calls --
for i, proj in ipairs(projects) do

    print("Generating " .. proj.name)

    project(proj.name)
    kind(proj.kind)

    if(os.target() == "windows" and proj.kind == "WindowedApp") then
        linkoptions({"/ENTRY:\"mainCRTStartup\""})
    end

    if(os.target() == "windows" and (proj.kind == "WindowedApp" or proj.kind == "ConsoleApp")) then
        linkoptions({"/SAFESEH:NO"})
    end

    language(proj.language)

    if proj.language == "C" then 
        exceptionhandling ("Off")
        rtti("Off")
    else
        exceptionhandling ("On")
        rtti("On")
    end

    -- files --

    local proj_files = proj.files

    if debug_print then 
        print("Files: \n" .. table.tostring(proj_files))
    end

    files(proj_files) -- we already delt with tags in preprocessing --

    -- inc dirs --

    local proj_inc_dirs = find_table(proj, "include_dirs")

    if debug_print then
        print("Inc Dirs: \n" .. table.tostring(proj_inc_dirs))
    end

    includedirs(proj_inc_dirs)

    -- buildopts --

    local proj_buildopts = find_table(proj, "buildoptions")

    if debug_print then
        print("Build Options: " .. table.tostring(proj_buildopts))
    end

    buildoptions(proj_buildopts)

    -- links -- 

    local proj_links = find_table(proj, "links")

    if debut_print then 
        print("Links: " .. table.tostring(proj_links))
    end

    links(proj_links)

    -- defines --

    local proj_defines = find_table(proj, "defines")

    if debug_print then
        print("Defines: " .. table.tostring(proj_defines))
    end

    defines(proj_defines)

end
print("</process premake>")


-- [ end ] --

print("</preamke>")
