UniformFloat = {}
setmetatable(UniformFloat, {__index = BaseComponent})

UniformFloat.new = function (varname)
    local this = BaseComponent.new(varname)
    setmetatable(this, {__index=UniformFloat})
    return this
end

function UniformFloat:Do()
    -- nothing to do
    self:UpdateValue()
end

function UniformFloat:uniform()
    local v = self.value
    return v.value
end
