Vec4ToFloat = {}
setmetatable(Vec4ToFloat, {__index = HiveBaseModule})

Vec4ToFloat.new = function (varname)
    local this = HiveBaseModule.new(varname)
    setmetatable(this, {__index=Vec4ToFloat})
    return this
end

function Vec4ToFloat:Do()
    -- nothing to do
    self:UpdateValue()
    return true
end

function Vec4ToFloat:value0()
    local v = self.value
    return v.vec4[1]
end

function Vec4ToFloat:value1()
    local v = self.value
    return v.vec4[2]
end

function Vec4ToFloat:value2()
    local v = self.value
    return v.vec4[3]
end

function Vec4ToFloat:value3()
    local v = self.value
    return v.vec4[4]
end