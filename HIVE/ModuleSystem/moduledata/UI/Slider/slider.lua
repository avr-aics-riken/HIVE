Slider = {}
setmetatable(Slider, {__index = HiveBaseModule})

Slider.new = function (varname)
    local this = HiveBaseModule.new(varname)
    setmetatable(this, {__index=Slider})
    return this
end

function Slider:Do()
    -- nothing to do
    self:UpdateValue()
    return true
end

function Slider:fval()
    local v = self.value
    return v.value
end
