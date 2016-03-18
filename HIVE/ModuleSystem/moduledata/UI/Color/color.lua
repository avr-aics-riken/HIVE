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
    return true
end

function Color:color()
    self:UpdateValue()
    local v = self.value
    local value = v.value
    return value
end
