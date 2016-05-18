Print = {}
setmetatable(Print, {__index = HiveBaseModule})

Print.new = function (varname)
    local this = HiveBaseModule.new(varname)
    this.property = {
        value = 0.0
    }
    setmetatable(this, {__index=Print})
    return this
end

function Print:Do()
    -- nothing to do
    self:UpdateValue()
    local v = self.value
    local value = v.value
    print(value .. "\n")
    return true
end
