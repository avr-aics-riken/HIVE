Vec2ToFloat = {}
setmetatable(Vec2ToFloat, {__index = HiveBaseModule})

Vec2ToFloat.new = function (varname)
    local this = HiveBaseModule.new(varname)
    setmetatable(this, {__index=Vec2ToFloat})
    return this
end

function Vec2ToFloat:Do()
    -- nothing to do
    self:UpdateValue()
    return true
end

function Vec2ToFloat:value0()
    local v = self.value
    return v.vec2[1]
end

function Vec2ToFloat:value1()
    local v = self.value
    return v.vec2[2]
end