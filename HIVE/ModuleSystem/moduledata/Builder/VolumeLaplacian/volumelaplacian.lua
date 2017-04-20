
VolumeLaplacian = {}
setmetatable(VolumeLaplacian, {__index = HiveBaseModule})

VolumeLaplacian.new = function (varname)
    local this = HiveBaseModule.new(varname)
    local vf = LoadModule("VolumeFilter")
	this.vf = vf
    setmetatable(this, {__index=VolumeLaplacian})
    return this
end

function VolumeLaplacian:Do()
    self:UpdateValue()
    
    local v = self.value
    if (v.srcvolume == nil) then
        return "Invalid volume"
    end
	self.vf:Laplacian(v.srcvolume)
    return true
end

function VolumeLaplacian:volume()
    return self.vf:VolumeData()
end
