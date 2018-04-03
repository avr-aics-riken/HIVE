
SURFACE = {}
setmetatable(SURFACE, {__index = HiveBaseModule})

SURFACE.new = function (varname)
    local this = HiveBaseModule.new(varname)
    this.render = nil
    setmetatable(this, {__index=SURFACE})
    return this
end

function SURFACE:Do()
    self.render = LoadModule("RenderPluginSURFACE")
    if self.render then
        return true
    else
        return false
    end
end

function SURFACE:renderer()
    return self.render;
end
