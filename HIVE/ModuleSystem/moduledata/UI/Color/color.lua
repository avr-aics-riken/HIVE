Color = {}
setmetatable(Color, {__index = HiveBaseModule})

Color.new = function (varname)
    local this = HiveBaseModule.new(varname)
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
