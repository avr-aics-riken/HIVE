MPIMemoryData = {}
setmetatable(MPIMemoryData, {__index = HiveBaseModule})

MPIMemoryData.new = function (varname)
    local this = HiveBaseModule.new(varname)
    this.property = {
        value = 0.0
    }
    setmetatable(this, {__index=MPIMemoryData})
    return this
end

function MPIMemoryData:Do()
    -- nothing to do
    self:UpdateValue()
    local v = self.value
    return true
end

function MPIMemoryData:data()
    JSON = require('dkjson')
    local v = self.value
    local data = getMemoryData(v.dataname)
    if data ~= nil then
        return JSON.encode(data)
    end
    return nil
end