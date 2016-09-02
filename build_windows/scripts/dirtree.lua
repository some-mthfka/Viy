require "lfs"

-- function dirtree(dir) by David Kastrup
function dirtree(dir)
    assert(dir and dir ~= "", "directory parameter is missing or empty")
    if string.sub(dir, -1) == "/" then
        dir=string.sub(dir, 1, -2)
    end

    local function yieldtree(dir)
        for entry in lfs.dir(dir) do
            if entry ~= "." and entry ~= ".." then
                entry=dir.."/"..entry
                local attr=lfs.attributes(entry)
                coroutine.yield(entry,attr)
                if attr.mode == "directory" then
                    yieldtree(entry)
                end
            end
        end
    end

    return coroutine.wrap(function() yieldtree(dir) end)
end

function get_library_path()
    library_path = lfs.currentdir().."/library"
    library_path = library_path:gsub('\\', "/")
    return library_path 
end
