MPIMemoryDataNames = {}
setmetatable(MPIMemoryDataNames, {__index = HiveBaseModule})

MPIMemoryDataNames.new = function (varname)
    local this = HiveBaseModule.new(varname)
    setmetatable(this, {__index=MPIMemoryDataNames})
    return this
end

function MPIMemoryDataNames:Do()
    -- nothing to do
    self:UpdateValue()
    local v = self.value
    return true
end

function MPIMemoryDataNames:datanames()
    JSON = require('dkjson')
    local names = getMemoryDataNames()
    if names ~= nil then
        return JSON.encode(names)
    end
    return nil 
end
