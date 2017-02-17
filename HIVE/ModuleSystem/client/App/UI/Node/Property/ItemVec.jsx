import React from "react";
import ReactDOM from "react-dom";
import Core from '../../../Core';

/**
 * ノードプロパティアイテム(vector)ビュー.
 */
export default class ItemVec extends React.Component {
	constructor(props) {
		super(props);

		this.state = {
			values : JSON.parse(JSON.stringify(this.props.initialParam.value)),
			onFrame : this.props.hasOwnProperty('store') ? this.props.store.hasCurrentKeyFrame(this.props.initialParam) : false
		};
		this.currentEdit = {
			index : -1,
			value : null
		};
		this.frameApplied = this.frameApplied.bind(this);
		this.keyBackGround = this.keyBackGround.bind(this);
	}

	width() {
		const len = this.props.initialParam.value.length;
		if (len === 4) {
			return "39px";
		} else {
			return "53px";
		}
	}

	keyBackGround() {
		if (this.state.onFrame) {
			return "rgb(19, 154, 165)";
		}
		return "white";
	}

	frameApplied(err, content, prop) {
		if (content.nodeVarname === this.props.initialParam.nodeVarname &&
			prop.name === this.props.initialParam.name) {
			if (prop.data.hasOwnProperty(this.props.store.getCurrentFrame())) {
				this.setState({
					onFrame	: true
				});
			} else {
				this.setState({
					onFrame	: false
				});
			}
		}
	}

	componentDidMount() {
		if (this.props.hasOwnProperty('store')) {
			this.props.store.on(Core.Constants.CURRENT_FRAME_APPLIED, this.frameApplied);
		}
	}

	componentWillUnmount() {
		if (this.props.hasOwnProperty('store')) {
			this.props.store.off(Core.Constants.CURRENT_FRAME_APPLIED, this.frameApplied);
		}
	}

	styles() {
        let border = ()=>{
            if(this.props.top && this.props.bottom){
                return {
                    borderRadius: "3px 3px 3px 3px",
                    letterSpacing: "-5px",
                    overflow: "hidden"
                };
            }else if(this.props.top){
                return {
                    borderBottom: "1px solid rgb(33, 187, 151)",
                    borderRadius: "3px 3px 0px 0px",
                    letterSpacing: "-5px",
                    overflow: "hidden"
                };
            }else if(this.props.bottom){
                return {
                    border: "none",
                    borderRadius: "0px 0px 3px 3px",
                    letterSpacing: "-5px",
                    overflow: "hidden"
                };
            }else{
                return {
                    borderBottom: "1px solid rgb(33, 187, 151)",
                    letterSpacing: "-5px",
                    overflow: "hidden"
                };
            }
        };
        return {
            view : border.bind(this)(),
            key : {
                backgroundColor: "rgb(84,84,84)",
                color : "white",
                fontSize: "11px",
                letterSpacing: "normal",
                textAlign: this.props.initialParam.name.match(/^\[\d\]$/) ? "right" : "left",
                padding: "1px",
				paddingLeft : "4px",
                width : "80px",
                verticalAlign: "middle",
                display: "inline-block",
                overflow: "hidden",
                textShadow: "0px 0px 3px black",
				float : "left"
            },
            value : {
                color : "#333",
                letterSpacing: "normal",
                //padding: "1px",
                width : "170px",
                verticalAlign: "middle",
                display: "inline-block"
            },
			addkey : {
				backgroundColor : this.keyBackGround.bind(this)(),
				borderRadius : "6px",
				width : "8px",
				height : "8px",
				marginTop : "6px",
				marginBottom : "6px",
				marginRight : "4px",
				cursor : "pointer",
				position : "absolute"
			},
            flex : {
                //paddingLeft: "3px"
                // display: "flex",
                // flexDirection: "row"
            },
            inputs: {
				outline: "0",
                border: "0px",
                borderRadius: "2px",
                marginLeft: "3px",
				marginTop: "1px",
				marginBottom: "1px",
                verticalAlign: "middle",
                width: this.width.bind(this)(),
				height: "19px",
                float: "left",
				border : "1px solid rgba(54, 196, 168, 0.0)"
                // flexGrow: 1
            }
        };
	}

	onChange(index) {
		return (ev) => {
			let val = ev.target.value;
			this.state.values[index] = val;
			this.currentEdit = {
				index : index,
				value : parseFloat(val)
			};
			this.setState({
				values : [].concat(this.state.values)
			});
		};
	}

	submit(ev) {
		if (this.currentEdit.value !== null && !isNaN(this.currentEdit.value) && this.currentEdit.index >= 0) {
			this.props.changeVecFunc(this.currentEdit.index, this.currentEdit.value);
		} else if (isNaN(this.currentEdit.value) && this.currentEdit.index >= 0) {
			this.state.values[this.currentEdit.index] = 0;
			this.setState({
				values : [].concat(this.state.values)
			});
		}
		ev.target.style.border = "none";
		ev.target.blur();
	}

	onKeyPress(ev) {
		if (ev.key === 'Enter') {
			this.submit.bind(this)(ev);
		}
	}

	onKeyDown(ev) {
		//ev.preventDefault();
		ev.stopPropagation();
	}
	
	onBlur(ev) {
		this.submit.bind(this)(ev);
		this.currentEdit = {
			index : -1,
			value : null
		};
	}

	onFocus(ev) {
		ev.target.style.border = "2px solid darkgreen";
	}

	valueElem() {
		const styles = this.styles.bind(this)();
		let values = this.props.initialParam.value.map( (val, key) => {
			return (<input style={styles.inputs}
						type="text"
						ref="text"
						onChange={this.onChange.bind(this)(key)}
						defaultValue={this.state.values[key]}
						value={this.state.values[key]}
						onKeyPress={this.onKeyPress.bind(this)}
						onKeyDown={this.onKeyDown.bind(this)}
						onBlur={this.onBlur.bind(this)}
						onFocus={this.onFocus.bind(this)}
						key={key}
					></input>);
		});
		return values;
	}

	onChangeKey(ev) {
		if (ev.button === 0 || ev.button === 2) {
			var hole = this.props.initialParam;
			hole.value = this.state.values;
			if (!this.state.onFrame === false) {
				this.props.deleteKeyFunc(this.props.initialParam);
			} else {
				this.props.changeKeyFunc(hole);
			}
			this.setState({
				onFrame : !this.state.onFrame
			});
		}
	}

	addKeyElem() {
		const styles = this.styles.bind(this)();
		if (this.props.initialParam.hasOwnProperty('anim') && !this.props.initialParam.anim) { return; }
		return (<span style={styles.addkey} onMouseDown={this.onChangeKey.bind(this)} />);
	}

	title() {
		return this.props.initialParam.hasOwnProperty('label') ? this.props.initialParam.label : this.props.initialParam.name;
	}

	render () {
		const styles = this.styles.bind(this)();
		return (<div style={styles.view}>
					<div style={styles.key}>
						{this.addKeyElem.bind(this)()}
						<span style={{marginLeft : "12px"}} title={this.title.bind(this)()}>{this.title.bind(this)()}</span>
					</div>
                    <div style={styles.value}>
                        <div style={styles.flex}>
                            {this.valueElem.bind(this)()}
                        </div>
                    </div>
				</div>);
	}
}
