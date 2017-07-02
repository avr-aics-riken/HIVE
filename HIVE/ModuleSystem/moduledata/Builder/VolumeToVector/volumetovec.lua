
VolumeToVector = {}
setmetatable(VolumeToVector, {__index = HiveBaseModule})

VolumeToVector.new = function (varname)
    local this = HiveBaseModule.new(varname)
    local vm = LoadModule("VolumeToVector")
	this.vm = vm
    setmetatable(this, {__index=VolumeToVector})
    return this
end

function VolumeToVector:Do()
    self:UpdateValue()
    
    local v = self.value
    if (v.srcvolume == nil) then
        return "Invalid volume"
    end
    
    self.vm:DivideNumber(v.divideNum[1], v.divideNum[2], v.divideNum[3])
	self.vm:Create(v.srcvolume)
    return true
end

function VolumeToVector:VectorData()
    return self.vm:VectorData()
end
