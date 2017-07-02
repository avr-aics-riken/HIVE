SavePDM = {}
setmetatable(SavePDM, {__index = HiveBaseModule})

SavePDM.new = function (varname)
    local this = HiveBaseModule.new(varname)
    setmetatable(this, {__index=SavePDM})
    this.saver = LoadModule("PdmSaver")
    return this
end

function SavePDM:executeSave()
    local v = self.value
    
    if v.pointdata == nil then
        return false
    end
    self.saver:SetPointData(v.pointdata)
    
    if v.only_export then
        if HIVE_EXPORTMODE then
            return self.saver:Save(v.filepath)
        else
            return true
        end
    else
        return self.saver:Save(v.filepath)
    end
end

function SavePDM:Do()
    self:UpdateValue()
    
    if mpiMode() == true then
        if mpiRank() == 0 then
            return self:executeSave()
        end
    else
        return self:executeSave()
    end
end

