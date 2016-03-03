Color = {}
setmetatable(Color, {__index = BaseComponent})

Color.new = function (varname)
    local this = BaseComponent.new(varname)
    this.property = {
        value = {1,1,1,1}
    }
    setmetatable(this, {__index=Color})
    return this
end

function Color:Do()
    -- nothing to do
end

function Color:color()
    local value = self.property.value
    if self.connection.value then
        value = self.connection.value
    end	
    return value
end
