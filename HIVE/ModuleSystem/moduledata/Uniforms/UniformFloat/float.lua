UniformFloat = {}
setmetatable(UniformFloat, {__index = HiveBaseModule})

UniformFloat.new = function (varname)
    local this = HiveBaseModule.new(varname)
    setmetatable(this, {__index=UniformFloat})
    return this
end

function UniformFloat:Do()
    -- nothing to do
    self:UpdateValue()
    return true
end

function UniformFloat:uniform()
    local v = self.value
    return { name = v.name, type = "float", value = v.value}
end
