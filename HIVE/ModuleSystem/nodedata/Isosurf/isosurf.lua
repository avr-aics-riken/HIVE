

Isosurf = {}
setmetatable(Isosurf, {__index = BaseComponent})

Isosurf.new = function (varname)
    local this = BaseComponent.new(varname)
    local vm = require("VolumeToMeshData")();
	this.vm = vm
    setmetatable(this, {__index=Isosurf})
    return this
end

function Isosurf:Do()
    self:UpdateValue()
    
    local v = self.value
    self.vm:Create(v.volume)
	self.vm:SetIsoValue(v.isovalue)
	self.vm:IsoSurface()
end

function Isosurf:MeshData()
    return self.vm:MeshData()
end
