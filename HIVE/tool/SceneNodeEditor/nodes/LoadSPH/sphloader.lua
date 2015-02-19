function LoadSPH(property)
	local sphloader
	sphloader = SPHLoader()
	sphloader:Load(property.filepath)
	return sphloader
end
