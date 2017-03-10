
VolumeElementFilter = {}
setmetatable(VolumeElementFilter, {__index = HiveBaseModule})

VolumeElementFilter.new = function (varname)
    local this = HiveBaseModule.new(varname)
    local vm = require("FloatsToFloat")();
	this.vm = vm
    setmetatable(this, {__index=VolumeElementFilter})
    return this
end

function VolumeElementFilter:Do()
    self:UpdateValue()
    
    local v = self.value
    if (v.srcvolume == nil) then
        return "Invalid volume"
    end
    
    local elem = v.element
	if elem == nil then
		elem = 0
	end
    
	self.vm:Create(v.srcvolume, elem)
    return true
end

function VolumeElementFilter:volume()
    return self.vm:VolumeData()
end
