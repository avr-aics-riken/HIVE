UniformVec3 = {}
setmetatable(UniformVec3, {__index = BaseComponent})

UniformVec3.new = function (varname)
    local this = BaseComponent.new(varname)
    this.property = {
        name = 'univec3',
        value = {0,0,0}               
    }
    setmetatable(this, {__index=UniformVec3})
    return this
end

function UniformVec3:Do()
    -- nothing to do
    self:UpdateValue()
end

function UniformVec3:uniform()
    local v = self.value    
    
    return {name=v.name, value=v.value, type='vec3'}
end

