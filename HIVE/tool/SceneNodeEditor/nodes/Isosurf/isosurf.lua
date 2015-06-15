function Isosurf(property)
	local vm = VolumeToMeshData();
	vm:Create(property.volume)
	vm:SetIsoValue(property.isovalue)
	vm:IsoSurface()
	return vm
end
