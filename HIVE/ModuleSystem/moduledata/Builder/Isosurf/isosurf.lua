

Isosurf = {}
setmetatable(Isosurf, {__index = HiveBaseModule})

Isosurf.new = function (varname)
    local this = HiveBaseModule.new(varname)
    local vm = LoadModule("VolumeToMeshData")
	this.vm = vm
    setmetatable(this, {__index=Isosurf})
    return this
end

function Isosurf:Do()
    self:UpdateValue()
    
    local v = self.value
    if (v.volume == nil) then
        return "Invalid volume"
    end
    
    self.vm:Create(v.volume)
	self.vm:SetIsoValue(v.isovalue)
	self.vm:IsoSurface()
    return true
end

function Isosurf:MeshData()
    return self.vm:MeshData()
end
