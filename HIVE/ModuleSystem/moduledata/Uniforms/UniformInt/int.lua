UniformInt = {}
setmetatable(UniformInt, {__index = HiveBaseModule})

UniformInt.new = function (varname)
    local this = HiveBaseModule.new(varname)
    setmetatable(this, {__index=UniformInt})
    return this
end

function UniformInt:Do()
    -- nothing to do
    self:UpdateValue()
    return true
end

function UniformInt:uniform()
    local v = self.value
    return { name = v.name, type = "int", value = v.value}
end
