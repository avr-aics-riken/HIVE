function SaveRAW(property)
	local volSaver = RawVolumeSaver()
	local appendFooter = false
	volSaver:SetVolumeData(property.volumedata)
	volSaver:Save(property.filepath, appendFooter)
end
