function SaveSPH(property)
	local sphSaver = SPHSaver()
	sphSaver:SetVolumeDatta(property.volumedata)
	sphSaver:Save(property.filepath)
end
