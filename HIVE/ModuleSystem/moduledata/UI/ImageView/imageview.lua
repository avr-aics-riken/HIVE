ImageView = {}
setmetatable(ImageView, {__index = HiveBaseModule})

ImageView.new = function (varname)
    local this = HiveBaseModule.new(varname)
    setmetatable(this, {__index=ImageView})
    return this
end

function ImageView:Do()
    -- nothing to do
    self:UpdateValue()
    print("selfvarname", self.varname)
    
    local v = self.value
    if (v.image == nil) then
        return false
    end
    local mode = 'png'
    -- image save
    local imageBuffer
    local imageBufferSize
    if v.ipcmode then
        mode = 'raw'
        -- image save
        imageBuffer = v.image:GetBuffer()
        imageBufferSize = v.image:GetSize()
    else
        -- image save
        imageBuffer = HIVE_ImageSaver:SaveMemory(5, v.image) --png
        imageBufferSize = HIVE_ImageSaver:MemorySize()
    end
    
    -- create metabinary
    --local w = v.screensize[1]
    --local h = v.screensize[2]
    local w = v.image:GetWidth()
    local h = v.image:GetHeight()
    print('rendersize=('.. w ..",".. h ..")", 'cancel=', tostring(HIVE_isRenderCanceled))
    
    local json = [[{
            "JSONRPC" : "2.0",
            "method" : "renderedImage",            
            "to" : ]] .. targetClientId ..[[,
            "param" : {
                "type" : "]] .. mode .. [[",
                "width" : "]] .. w .. [[",
                "height" : "]] .. h .. [[",
                "canceled": ]] .. tostring(HIVE_isRenderCanceled) .. [[,
                "varname": "]] .. self.varname .. [["
            },
            "id":0
    }]]
    HIVE_metabin:Create(json, imageBuffer, imageBufferSize)
    --print('JSON=', json, 'size=', imageBufferSize)
    -- send
    print('ipcmode', v.ipcmode)        
    if v.ipcmode then       
        if self.network_ipc == nil then
            local ipcAddress = 'ipc:///tmp/HIVE_IPC_' .. self.varname
            print('IPC open=', ipcAddress);
	        self.network_ipc = require("Network").Connection()
	        local ipcr = self.network_ipc:Connect(ipcAddress)
	        print('IPC ret=', ipcr)
        end
        print('IPC->', self.network_ipc)
        self.network_ipc:SendBinary(HIVE_metabin:BinaryBuffer(), HIVE_metabin:BinaryBufferSize())
        print('IPC SEND!!!!!!!!!!!')			   

    else
        network:SendBinary(HIVE_metabin:BinaryBuffer(), HIVE_metabin:BinaryBufferSize())
        print('WEBSOCKET SEND!!!!!!!!!!!')			   

    end
    return true
end
