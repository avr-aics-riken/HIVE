CDM2KVSPoint = {}
setmetatable(CDM2KVSPoint, {__index = HiveBaseModule})

CDM2KVSPoint.new = function (varname)
    local this = HiveBaseModule.new(varname)
    this.kvsvolumeconv = LoadModule('KVSCartesianVolumeConverter')
    setmetatable(this, {__index=CDM2KVSPoint})
    return this
end

function CDM2KVSPoint:Do()
    self:UpdateValue()
    self.kvsvolumeconv:setVolumeBuffer(self.value.VolumeData)
    return true
end

function CDM2KVSPoint:object()
    return self.kvsvolumeconv:getKVSVolumeData()
end
