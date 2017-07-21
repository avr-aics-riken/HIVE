Time = {}
setmetatable(Time, {__index = HiveBaseModule})

Time.new = function (varname)
    local this = HiveBaseModule.new(varname)
    this.property = {
        value = {1,1,1,1}
    }
    setmetatable(this, {__index=Time})
    return this
end

function Time:Do()
    self:UpdateValue()
    -- nothing to do
    return true
end

function Time:time()
    local v = self.value
    local value = v.value
    return value
end

function Time:timestring()
    local v = self.value
    local value = v.value
    return tostring(value)
end

