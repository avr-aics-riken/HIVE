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
end

function UniformVec3:uniform()
    local name = self.property.name
    local value = self.property.value
    if self.connection.name then
        name = self.connection.name
    end
    if self.connection.value then
        name = self.connection.value
    end
    
    return {name=name, value=value, type='vec3'}
end

