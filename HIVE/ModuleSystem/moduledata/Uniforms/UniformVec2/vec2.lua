UniformVec2 = {}
setmetatable(UniformVec2, {__index = BaseComponent})

UniformVec2.new = function (varname)
    local this = BaseComponent.new(varname)
    this.property = {
        name = 'univec2',
        value = {0,0,0}
    }
    setmetatable(this, {__index=UniformVec2})
    return this
end

function UniformVec2:Do()
    -- nothing to do
    self:UpdateValue()
    return true
end

function UniformVec2:uniform()
    local v = self.value

    return {name=v.name, value=v.value, type='vec2'}
end
