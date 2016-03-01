UniformVec3 = {}
setmetatable(UniformVec3, {__index = BaseComponent})

UniformVec3.new = function (varname)
    local this = BaseComponent.new(varname)
    this.property = {
        name = 'univec3',
        value = {0,0,0}               
    }
    return this
end

function UniformVec3:Do()
    -- nothing to do
end

function UniformVec3:Uniform()
    local name = this.property.name
    local value = this.property.value
    if this.connection.name then
        name = this.connection.name
    end
    if this.connection.value then
        name = this.connection.value
    end
    
    return {name=name, value=value, type='vec3'}
end

