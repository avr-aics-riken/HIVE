FloatToVec4 = {}
setmetatable(FloatToVec4, {__index = HiveBaseModule})

FloatToVec4.new = function (varname)
    local this = HiveBaseModule.new(varname)
    setmetatable(this, {__index=FloatToVec4})
    return this
end

function FloatToVec4:Do()
    -- nothing to do
    self:UpdateValue()
    return true
end

function FloatToVec4:vec4()
    local v = self.value
    return { v.value0, v.value1, v.value2, v.value3 }
end
