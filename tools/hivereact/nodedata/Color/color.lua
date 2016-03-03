Color = {}
setmetatable(Color, {__index = BaseComponent})

Color.new = function (varname)
    local this = BaseComponent.new(varname)
    this.property = {
        name = 'color',
        value = {0,0,0,1}
    }
    setmetatable(this, {__index=Color})
    return this
end

function Color:Do()
    -- nothing to do
end

function Color:color()
    local name = self.property.name
    local value = self.property.value
    if self.connection.name then
        name = self.connection.name
    end
    if self.connection.value then
        name = self.connection.value
    end
	print(self.connection)

    return {name=name, value=value, type='vec4'}
end
