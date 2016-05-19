FloatToVec2 = {}
setmetatable(FloatToVec2, {__index = HiveBaseModule})

FloatToVec2.new = function (varname)
    local this = HiveBaseModule.new(varname)
    setmetatable(this, {__index=FloatToVec2})
    return this
end

function FloatToVec2:Do()
    -- nothing to do
    self:UpdateValue()
    return true
end

function FloatToVec2:vec2()
    local v = self.value
    return { v.value0, v.value1 }
end
