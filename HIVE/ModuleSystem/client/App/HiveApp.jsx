import ModuleEntry from './ModuleEntry.jsx';

import React from 'react';
import ReactDOM from "react-dom";
import Core from './Core';
import Hive from './HIVE';
import Node from "./UI/Node";
import Panel from "./UI/Panel";
import Menu from "./UI/Menu";
import Dialog from "./UI/Dialog";
import MenuTop from "./UI/Menu/MenuTop.jsx";
import TimeSlider from "./UI/TimeSlider";
import Splitter from "./UI/Splitter";
import HoverNodeCreate from "./UI/Menu/HoverNodeCreate.jsx";
import Constants from "./Core/Constants.jsx"
import ConsoleOutput from "./UI/ConsoleOutput";
import FileBrowser from "./UI/FileBrowser"

export default class HiveApp extends React.Component {
    constructor (props) {
        super(props);

        this.store = new Core.Store();
        this.action = new Core.Action(this.store.getDispatchToken());
        this.listVisiblity = false;

		this.nodeStore = new Node.Store(this.action.dispatcher, this.store);
		this.nodeAction = new Node.Action(this.action.dispatcher, this.nodeStore.getDispatchToken());

        this.state = {
            layoutType: 'all',
            consoleOutputVisible: false,
            listVisible: false,
            listPos: [window.innerWidth / 2, window.innerHeight / 2 - 150],
			filebrowser : false,
			labeldialog : false,
            messagedialog : false,
        };

		this.ctrlKeyDown = false;
        this.onDragOver = this.onDragOver.bind(this);
        this.onDrop = this.onDrop.bind(this);
        this.onClick = this.onClick.bind(this);
        this.onDblClick = this.onDblClick.bind(this);
        this.onKeyDown = this.onKeyDown.bind(this);
		this.onMouseMove = this.onMouseMove.bind(this);
        this.onKeyUp = this.onKeyUp.bind(this);
        this.componentDidMount = this.componentDidMount.bind(this);
        this.loadFile = this.loadFile.bind(this);
        this.setFocusTarget = this.setFocusTarget.bind(this);
        this.hoverHidden = this.hoverHidden.bind(this);
        this.hoverGenerator = this.hoverGenerator.bind(this);
        this.setHoverPosition = this.setHoverPosition.bind(this);

		this.removeEvents = this.removeEvents.bind(this);
		this.assignEvents = this.assignEvents.bind(this);

		this.onFilebrowserOK = this.onFilebrowserOK.bind(this);
		this.onFilebrowserCancel = this.onFilebrowserCancel.bind(this);
		this.onLabelDialogOK = this.onLabelDialogOK.bind(this);
		this.onLabelDialogCancel = this.onLabelDialogCancel.bind(this);
        this.onMessageDialogOK = this.onMessageDialogOK.bind(this);
		this.onMessageDialogCancel = this.onMessageDialogCancel.bind(this);
		
		this.mousePos = {x : 0, y : 0};

        this.store.on(Constants.LAYOUT_CHANGED, (val) => {
            console.log('LAYOUT_CHANGED', val);
			this.removeEvents();
            this.setState({"layoutType": val});
        });
        this.store.on(Constants.CONSOLEOUTPUT_SHOW, (val) => {
            console.log('CONSOLEOUTPUT_SHOW', val);
            this.setState({"consoleOutputVisible": val});
        });
    }

	removeEvents() {
		let currentMode = this.layoutMode.bind(this)(this.state.layoutType);
		let dropTarget = ReactDOM.findDOMNode(this.refs["dropTarget" + String(currentMode)]);
		let hoverTarget = ReactDOM.findDOMNode(this.refs["hoverTarget" + String(currentMode)]);

		if (dropTarget) {
			dropTarget.removeEventListener('dragover', this.onDragOver, false);
			dropTarget.removeEventListener('drop', this.onDrop, false);
		}
		if (hoverTarget) {
			window.removeEventListener('click', this.onClick, false);
			hoverTarget.removeEventListener('dblclick', this.onDblClick, false);
		}
		this.dropTarget = null;
		this.hoverTarget = null;
	}

	assignEvents() {
		let currentMode = this.layoutMode.bind(this)(this.state.layoutType);
		if (!this.dropTarget) {
			this.dropTarget = ReactDOM.findDOMNode(this.refs["dropTarget" + String(currentMode)]);
			if (this.dropTarget) {
				this.dropTarget.addEventListener('dragover', this.onDragOver, false);
				this.dropTarget.addEventListener('drop', this.onDrop, false);
			}
		}
		if (!this.hoverTarget) {
			this.hoverTarget = ReactDOM.findDOMNode(this.refs["hoverTarget" + String(currentMode)]);
			if (this.hoverTarget) {
				window.addEventListener('click', this.onClick, false);
				this.hoverTarget.addEventListener('dblclick', this.onDblClick, false);
			}
		}
	}

    componentDidMount() {
		this.assignEvents();
		window.addEventListener('mousemove', this.onMouseMove);
		window.addEventListener('keydown', this.onKeyDown);
		window.addEventListener('keyup', this.onKeyUp);
		this.store.on(Constants.OPEN_FILE_BROWSER, (err, key) => {
			this.filebrowserKey = key;
			this.setState({ filebrowser: true});
		});

		this.store.on(Constants.OPEN_LABEL_DIALOG, (err, key, callback) => {
			this.labeldialogKey = key;
			let currentMode = this.layoutMode.bind(this)(this.state.layoutType);
			let dialog = "labelDialog" + String(currentMode);
			if (this.refs.hasOwnProperty(dialog)) {
				this.refs[dialog].setState({label : key})
			}
			this.labeldialogCallback = callback;
			this.setState({ labeldialog: true});
		});
        
		this.store.on(Constants.OPEN_MESSAGE_DIALOG, (err, key, callback) => {
			this.messagedialogKey = key;
			let currentMode = this.layoutMode.bind(this)(this.state.layoutType);
			let dialog = "messageDialog" + String(currentMode);
			if (this.refs.hasOwnProperty(dialog)) {
				this.refs[dialog].setState({label : key})
			}
			this.messagedialogCallback = callback;
			this.setState({ messagedialog: true});
		});
    }

	componentWillUnmount() {
		this.removeEvents();
		window.removeEventListener('mousemove', this.onMouseMove);
		window.removeEventListener('keydown', this.onKeyDown);
		window.removeEventListener('keyup', this.onKeyUp);
	}

	componentDidUpdate() {
		this.assignEvents(this.state.layoutType);
	}
    onDragOver(eve){
        eve.stopPropagation();
        eve.preventDefault();
    }

    onDrop(eve){
        eve.stopPropagation();
        eve.preventDefault();
        if(eve.dataTransfer.files && eve.dataTransfer.files.length > 0){
            this.loadFile(eve.dataTransfer.files[0]);
        }
        return false;
    }

    // キーダウンイベントのターゲットは Window
    onKeyDown(eve){
		let keyCode = (eve.which || eve.keyCode);
		if (keyCode == 116 // F5 
				|| (this.ctrlKeyDown && keyCode == 82)) // Ctrl R or Command R
		{
			// Reboot hive
			this.action.openMessageDialog("Is it OK to reboot HIVE?", (err, key) => {
				if (!err) {
					this.action.rebootHive();
				}
			});
			eve.preventDefault();
		} else if (keyCode == 17 || keyCode == 224 || keyCode == 91 || keyCode == 93) {
			this.ctrlKeyDown = true;
		}
        switch(eve.keyCode){
            case 27:
                this.hoverHidden();
                break;
            case 32:
				let currentMode = this.layoutMode.bind(this)(this.state.layoutType);
				let hoverTarget = ReactDOM.findDOMNode(this.refs["hoverTarget" + String(currentMode)]);
				let rect = hoverTarget.getBoundingClientRect();
				if (this.mousePos.x >= rect.left && this.mousePos.x <= rect.right &&
					this.mousePos.y >= rect.top && this.mousePos.y <= rect.bottom)
				{
					eve.preventDefault();
					this.listVisiblity = true;
					this.setState({
						listVisible: true,
						listPos: [
							Math.min(this.mousePos.x, rect.right - 270),  
							Math.min(this.mousePos.y, rect.bottom - 270)]
					});
					setTimeout((()=>{
						this.setHoverPosition();
						var e = ReactDOM.findDOMNode(this.focusTarget.refs.suggest.input);
						e.focus();
					}).bind(this), 30);
				}
            default:
                break;
        }
    }
	
	onMouseMove(eve) {
		this.mousePos = {
			x : eve.pageX,
			y : eve.pageY
		};
	}
	
	onKeyUp(eve) {
		let keyCode = (eve.which || eve.keyCode);
		if (keyCode == 17 || keyCode == 224 || keyCode == 91 || keyCode == 93) {
			this.ctrlKeyDown = false;
		}
	}

    onClick(eve) {
        if (eve.button === 0) {
            if (this.listVisiblity) {
                this.listVisiblity = false;
                this.setState({
                    listVisible: this.listVisiblity
                });
            }
        }
    }

    onDblClick(eve){
        if (eve.button === 0) {
            // let x = eve.currentTarget.scrollLeft + eve.clientX - eve.currentTarget.getBoundingClientRect().left;
            // let y = eve.currentTarget.scrollTop + eve.clientY - eve.currentTarget.getBoundingClientRect().top;
            let x = eve.pageX;
            let y = eve.pageY;
			let currentMode = this.layoutMode.bind(this)(this.state.layoutType);
			let hoverTarget = ReactDOM.findDOMNode(this.refs["hoverTarget" + String(currentMode)]);
			let rect = hoverTarget.getBoundingClientRect();
            this.listVisiblity = !this.listVisiblity;
            this.setState({
                listVisible: this.listVisiblity,
                listPos: [
						Math.min(x, rect.right - 270),  
						Math.min(y, rect.bottom - 270)]
            });
            if(this.listVisiblity){
                setTimeout((()=>{
                    var e = ReactDOM.findDOMNode(this.focusTarget.refs.suggest.input);
                    e.focus();
                }).bind(this), 30);
            }
        }
    }

    loadFile(file){
        var reader = new FileReader();
        reader.onload = function(){
            let data = (JSON.parse(reader.result));
			this.action.load(data);
        }.bind(this);
        reader.readAsText(file);
    }

    menu() {
        return this.state.isInitialized ? (<Menu.View store={this.store} action={this.action} />) : (<div />);
    }

    // ========================================================================
    // この関数をフォーカスしたい子要素に渡して呼んでもらう
    setFocusTarget(element){
        this.focusTarget = element;
    }

    hoverHidden(){
        this.listVisiblity = false;
        this.setState({listVisible: false});
    }

    setHoverPosition(){
        let el, pos, x, y, w, h;
		if (this.hoverTarget ) {
	        el = this.hoverTarget;
	        pos = el.getBoundingClientRect();
	        w = el.clientWidth;
	        h = el.clientHeight;
	        x = w / 2 + pos.left - 100;
	        y = h / 2 + pos.top - 150;
	        this.setState({listPos: [x, y]});
		}
    }

    hoverGenerator(){
        if (this.state.listVisible){
            return (
                <HoverNodeCreate
                    store={this.store}
                    action={this.action}
                    visibility={this.state.listVisible}
                    position={this.state.listPos}
                    focusFunction={this.setFocusTarget.bind(this)}
                    hiddenFunction={this.hoverHidden}
                    ref="creator"
                />
            );
        }
    }
    // ========================================================================

	onFilebrowserOK(value) {
		this.action.okFileBrowser(this.filebrowserKey, value);
		this.setState({
			filebrowser : false
		});
	}

	onFilebrowserCancel(value) {
		this.setState({
			filebrowser : false
		});
	}

	onLabelDialogOK(value) {
		this.labeldialogCallback(null, value);
		this.action.okLabelDialog(this.labeldialogKey, value);
		this.setState({
			labeldialog : false
		});
	}

	onLabelDialogCancel(value) {
		this.setState({
			labeldialog : false
		});
	}
    
	onMessageDialogOK(value) {
		this.messagedialogCallback(null, value);
		this.action.okMessageDialog(this.messagedialogKey, value);
		this.setState({
			messagedialog : false
		});
	}

	onMessageDialogCancel(value) {
		this.setState({
			messagedialog : false
		});
	}

	layoutMode(layoutType) {
		if (layoutType === "all") { return 2; }
		else if (layoutType === "node") { return 1; }
		else if (layoutType === "panel") { return 0; }
		else if (layoutType === "panelnode") { return 3; }
		return 2;
	}

    render() {
        let layoutMode = this.layoutMode.bind(this)(this.state.layoutType);

        switch(layoutMode){
			case 3:
                return (
                    <div ref="dropTarget3">
                        <Splitter split="horizontal" secondPaneSize="150" lockSecondPane={true} dontmove={false} overflow2='hidden'>
                            <Splitter split="vertical" defaultSize="275" dontmove={true} overflow2="hidden">
                                <Menu.View store={this.store} action={this.action} layoutType={layoutMode} />
                                <div ref="hoverTarget3" style={{position:"absolute",width:"100%",height:"100%"}}>
                                    <Node.View store={this.store} action={this.action} nodeStore={this.nodeStore} nodeAction={this.nodeAction} isPanelNodeMode={true} />
                                </div>
                            </Splitter>
                            <TimeSlider.View store={this.store} action={this.action} />
                        </Splitter>
                        {this.hoverGenerator()}
                        <MenuTop lock={this.state.filebrowser || this.state.labeldialog || this.state.messagedialog} store={this.store} action={this.action} consoleShow={this.state.consoleOutputVisible}/>
						<FileBrowser.View
							display={this.state.filebrowser}
							okFunc={this.onFilebrowserOK}
							cancelFunc={this.onFilebrowserCancel}
							store={this.store}
							action={this.action} />
						<Node.LabelDialog ref="labelDialog3"
							store={this.store}
							action={this.action}
							display={this.state.labeldialog}
							okFunc={this.onLabelDialogOK}
							cancelFunc={this.onLabelDialogCancel}
							initialValue={this.labeldialogKey}
							/>
						<Dialog.MessageDialog ref="messageDialog3"
							store={this.store}
							action={this.action}
							display={this.state.messagedialog}
							okFunc={this.onMessageDialogOK}
							cancelFunc={this.onMessageDialogCancel}
							initialValue={this.messagedialogKey}
							/>
                        <ConsoleOutput store={this.store} show={this.state.consoleOutputVisible}/>
                    </div>
                );
                break;
            case 2:
                return (
                    <div ref="dropTarget2">
                        <Splitter split="horizontal" secondPaneSize="150" lockSecondPane={true} dontmove={false} overflow2='hidden'>
                            <Splitter split="vertical" defaultSize="275" dontmove={true}>
                                <Menu.View store={this.store} action={this.action} layoutType={layoutMode} />
                                <Splitter split="vertical" minSize="50" overflow1="hidden" overflow2='hidden'>
                                    <div ref="hoverTarget2" style={{position:"absolute",width:"100%",height:"100%"}}>
                                        <Node.View store={this.store} action={this.action} nodeStore={this.nodeStore} nodeAction={this.nodeAction} isPanelNodeMode={false} />
                                    </div>
                                    <div>
                                        <Panel.View store={this.store} action={this.action} />
                                    </div>
                                </Splitter>
                            </Splitter>
                            <TimeSlider.View store={this.store} action={this.action} />
                        </Splitter>
                        {this.hoverGenerator()}
                        <MenuTop lock={this.state.filebrowser || this.state.labeldialog || this.state.messagedialog} store={this.store} action={this.action} consoleShow={this.state.consoleOutputVisible}/>
						<FileBrowser.View
							display={this.state.filebrowser}
							okFunc={this.onFilebrowserOK}
							cancelFunc={this.onFilebrowserCancel}
							store={this.store}
							action={this.action} />
						<Node.LabelDialog ref="labelDialog2"
							store={this.store}
							action={this.action}
							display={this.state.labeldialog}
							okFunc={this.onLabelDialogOK}
							cancelFunc={this.onLabelDialogCancel}
							initialValue={this.labeldialogKey}
							/>
						<Dialog.MessageDialog ref="messageDialog2"
							store={this.store}
							action={this.action}
							display={this.state.messagedialog}
							okFunc={this.onMessageDialogOK}
							cancelFunc={this.onMessageDialogCancel}
							initialValue={this.messagedialogKey}
							/>
                        <ConsoleOutput store={this.store} show={this.state.consoleOutputVisible}/>
                    </div>
                );
                break;
            case 1:
                return (
                    <div ref="dropTarget1">
                        <Splitter split="horizontal" secondPaneSize="150" lockSecondPane={true} dontmove={false} overflow2='hidden'>
                            <Splitter split="vertical" defaultSize="275" dontmove={true} overflow2="hidden">
                                <Menu.View store={this.store} action={this.action} layoutType={layoutMode} />
                                <div ref="hoverTarget1" style={{position:"absolute",width:"100%",height:"100%"}}>
                                    <Node.View store={this.store} action={this.action} nodeStore={this.nodeStore} nodeAction={this.nodeAction} isPanelNodeMode={false} />
                                </div>
                            </Splitter>
                            <TimeSlider.View store={this.store} action={this.action} />
                        </Splitter>
                        {this.hoverGenerator()}
                        <MenuTop lock={this.state.filebrowser || this.state.labeldialog || this.state.messagedialog} store={this.store} action={this.action} consoleShow={this.state.consoleOutputVisible}/>
						<FileBrowser.View
							display={this.state.filebrowser}
							okFunc={this.onFilebrowserOK}
							cancelFunc={this.onFilebrowserCancel}
							store={this.store}
							action={this.action} />
						<Node.LabelDialog ref="labelDialog1"
							store={this.store}
							action={this.action}
							display={this.state.labeldialog}
							okFunc={this.onLabelDialogOK}
							cancelFunc={this.onLabelDialogCancel}
							initialValue={this.labeldialogKey}
							/>
						<Dialog.MessageDialog ref="messageDialog1"
							store={this.store}
							action={this.action}
							display={this.state.messagedialog}
							okFunc={this.onMessageDialogOK}
							cancelFunc={this.onMessageDialogCancel}
							initialValue={this.messagedialogKey}
							/>
                        <ConsoleOutput store={this.store} show={this.state.consoleOutputVisible}/>
                    </div>
                );
                break;
            case 0:
            default:
                return (
                    <div ref="dropTarget0">
                        <Splitter split="horizontal" secondPaneSize="150" lockSecondPane={true} dontmove={false} overflow1='hidden' overflow2='hidden'>
                            <Panel.View store={this.store} action={this.action} />
                            <TimeSlider.View store={this.store} action={this.action} />
                        </Splitter>
                        {this.hoverGenerator()}
                        <MenuTop lock={this.state.filebrowser || this.state.labeldialog || this.state.messagedialog} store={this.store} action={this.action} consoleShow={this.state.consoleOutputVisible}/>
                        <ConsoleOutput store={this.store} show={this.state.consoleOutputVisible}/>
                    </div>
                );
                break;
        }
    }
}
