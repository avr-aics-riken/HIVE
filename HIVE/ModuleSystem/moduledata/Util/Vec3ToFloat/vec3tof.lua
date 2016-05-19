Vec3ToFloat = {}
setmetatable(Vec3ToFloat, {__index = HiveBaseModule})

Vec3ToFloat.new = function (varname)
    local this = HiveBaseModule.new(varname)
    setmetatable(this, {__index=Vec3ToFloat})
    return this
end

function Vec3ToFloat:Do()
    -- nothing to do
    self:UpdateValue()
    return true
end

function Vec3ToFloat:value0()
    local v = self.value
    return v.vec3[1]
end

function Vec3ToFloat:value1()
    local v = self.value
    return v.vec3[2]
end

function Vec3ToFloat:value2()
    local v = self.value
    return v.vec3[3]
end