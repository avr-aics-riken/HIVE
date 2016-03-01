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
end

function UniformVec4:uniform()
    local name = self.property.name
    local value = self.property.value
    if self.connection.name then
        name = self.connection.name
    end
    if self.connection.value then
        name = self.connection.value
    end
    
    return {name=name, value=value, type='vec4'}
end

