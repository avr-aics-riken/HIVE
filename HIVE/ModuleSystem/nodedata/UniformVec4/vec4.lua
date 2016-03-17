UniformVec4 = {}
setmetatable(UniformVec4, {__index = BaseComponent})

UniformVec4.new = function (varname)
    local this = BaseComponent.new(varname)
    this.property = {
        name = 'univec4',
        value = {0,0,0,0}               
    }
    setmetatable(this, {__index=UniformVec4})
    return this
end

function UniformVec4:Do()
    -- nothing to do
    self:UpdateValue()
    return true
end

function UniformVec4:uniform()
    local v = self.value    
    return {name=v.name, value=v.value, type='vec4'}
end

