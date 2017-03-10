
VolumeNorm = {}
setmetatable(VolumeNorm, {__index = HiveBaseModule})

VolumeNorm.new = function (varname)
    local this = HiveBaseModule.new(varname)
    local vf = require("VolumeFilter")();
	this.vf = vf
    setmetatable(this, {__index=VolumeNorm})
    return this
end

function VolumeNorm:Do()
    self:UpdateValue()
    
    local v = self.value
    if (v.srcvolume == nil) then
        return "Invalid volume"
    end
    
	self.vf:Norm(v.srcvolume)
    return true
end

function VolumeNorm:volume()
    return self.vf:VolumeData()
end
