Slider = {}
setmetatable(Slider, {__index = BaseComponent})

Slider.new = function (varname)
    local this = BaseComponent.new(varname)
    setmetatable(this, {__index=Slider})
    return this
end

function Slider:Do()
    -- nothing to do
    self:UpdateValue()
end

function Slider:uniform()
    local v = self.value
    return v.value
end
