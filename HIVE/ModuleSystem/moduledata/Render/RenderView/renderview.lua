
RenderView = {}
setmetatable(RenderView, {__index = HiveBaseModule})

RenderView.new = function (varname)
    local this = HiveBaseModule.new(varname)
    this.cam = Camera()    
    this.property = {
        screensize = {512, 512},
        position = {0,0,300},
        target = {0,0,0},
        up = {0,1,0},
        fov = 60,
        clearcolor = {0,0,0,1},
        color_file = "",
        depth_file = "",
        ipcmode = false
    }
    this.network_ipc = nil
    
    setmetatable(this, {__index=RenderView})
    return this
end

function RenderView:Do()
    self:UpdateValue()
    local v = self.value
    
    self.cam:SetScreenSize(v.rendersize[1], v.rendersize[2])--v.screensize[1], v.screensize[2])
    self.cam:SetFilename(v.color_file)
    self.cam:SetDepthFilename(v.depth_file)
    self.cam:ClearColor(v.clearcolor[1],v.clearcolor[2],v.clearcolor[3],v.clearcolor[4])
    self.cam:LookAt(
        v.position[1], v.position[2], v.position[3],
        v.target[1],   v.target[2],   v.target[3],
        v.up[1],       v.up[2],       v.up[3],
        v.fov
    )
    
    local temp = {}
    local targetcam
-- For Object
    if v.RenderObject then
        temp = v.RenderObject
    end
        
-- For Camera
    if v.Camera then
        temp[#temp + 1] = v.Camera
        targetcam = v.Camera
    else
        temp[#temp + 1] = self.cam
        targetcam = self.cam
    end

-- dump
--[[
    for i,v in pairs(temp) do
         print('RenderObject->', v);   
    end
--]]
--[[
    if v.screensize[1] / 10 < v.rendersize[1] then
        render(temp, HIVE_fetchEvent)
    else
    ]]
        render(temp)
    --end
    
    if network == nil and HIVE_metabin == nil then -- no UI mode        
        return true
    end 
    
    local mode = 'jpg'
    -- image save
    local imageBuffer
    local imageBufferSize
    if self.property.ipcmode then
        mode = 'raw'
        -- image save
        local img = targetcam:GetImageBuffer()	
        imageBuffer = img:GetBuffer()
        imageBufferSize = img:GetSize()
    else    
        -- image save
        imageBuffer = HIVE_ImageSaver:SaveMemory(1, targetcam:GetImageBuffer())
        imageBufferSize = HIVE_ImageSaver:MemorySize()
    end
    
    -- create metabinary
    --local w = v.screensize[1]
    --local h = v.screensize[2]
    local w = v.rendersize[1]
    local h = v.rendersize[2]
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
    print('ipcmode', self.property.ipcmode)        
    if self.property.ipcmode then       
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


function RenderView:Camera()
    return self.cam
end

function RenderView:Image()
    return self.cam:GetImageBuffer()
end