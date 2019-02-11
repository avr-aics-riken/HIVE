PDM2KVSPoint = {}
setmetatable(PDM2KVSPoint, {__index = HiveBaseModule})

PDM2KVSPoint.new = function (varname)
    local this = HiveBaseModule.new(varname)
    this.kvspointconv = LoadModule('KVSPointConverter')
    setmetatable(this, {__index=PDM2KVSPoint})
    return this
end

function PDM2KVSPoint:Do()
    self:UpdateValue()
    self.kvspointconv:setPointBuffer(self.value.PointData)
    return true
end

function PDM2KVSPoint:object()
    return self.kvspointconv:getKVSPointData()
end
