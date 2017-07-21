Math = {}
setmetatable(Math, {__index = HiveBaseModule})

Math.new = function (varname)
    local this = HiveBaseModule.new(varname)
    setmetatable(this, {__index=Math});
    this.res = 0;
    return this
end

function Math:Do()
    self:UpdateValue()
    local v = self.value;
    local value0 = v.value0;
    local value1 = v.value1;
    local ope = v.operation;
    self.res = loadstring( string.format([[
        local value0 = %d;
        local value1 = %d;
        return (]] .. ope .. [[);]], value0, value1) )();
    return true
end

function Math:result()
    return self.res;
end
