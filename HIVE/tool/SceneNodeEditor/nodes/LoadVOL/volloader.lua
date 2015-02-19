function LoadVOL(property)
	local volloader
	volloader = VOLLoader()
	volloader:Load(property.filepath)
	return volloader
end
