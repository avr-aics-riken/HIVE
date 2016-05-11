FloatToVec3 = {}
setmetatable(FloatToVec3, {__index = HiveBaseModule})

FloatToVec3.new = function (varname)
    local this = HiveBaseModule.new(varname)
    setmetatable(this, {__index=FloatToVec3})
    return this
end

function FloatToVec3:Do()
    -- nothing to do
    self:UpdateValue()
    return true
end

function FloatToVec3:vec3()
    local v = self.value
    return { v.value0, v.value1, v.value2 }
end
