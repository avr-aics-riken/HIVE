function LoadVTK(property)
	local vtk = VTKLoader()
	 -- Need to specify field name. The last param is whether apply byteswap or not. 
	vtk:Load(property.filename, property.dirpath, property.fieldname, true)
	return vtk
end
