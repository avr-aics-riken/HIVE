Platform = {}
setmetatable(Platform, {__index = HiveBaseModule})

Platform.new = function (varname)
    local this = HiveBaseModule.new(varname)
    setmetatable(this, {__index=Platform})
    return this
end

function Platform:Do()
    -- nothing to do
    self:UpdateValue()
    local v = self.value
    return true
end

function Platform:platform()
    return platform()
end

function Platform:dllextension()
    return dllExtension()
end

function Platform:endian()
    return endian()
end