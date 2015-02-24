function Isosurf(property)
	local vm = VolumeToMesh();
	vm:Create(property.volume, property.isovalue)
	--vm:SetIsovalue(property.isovalue)
	return vm
end
