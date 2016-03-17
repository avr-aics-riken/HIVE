UniformTexture = {}
setmetatable(UniformTexture, {__index = BaseComponent})

UniformTexture.new = function (varname)
    local this = BaseComponent.new(varname)
    setmetatable(this, {__index=UniformTexture})
    return this
end

function UniformTexture:Do()
    -- nothing to do
    self:UpdateValue()
    return true
end

function UniformTexture:uniform()
    local v = self.value
    return {name=v.name, value=v.image, type='texture'}
end
