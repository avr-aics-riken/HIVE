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
			values : this.props.initialParam.value
		};
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
                fontSize: "smaller",
                letterSpacing: "normal",
                textAlign: this.props.initialParam.name.match(/^\[\d\]$/) ? "right" : "left",
                padding: "1px",
                width : "80px",
                verticalAlign: "middle",
                display: "inline-block",
                overflow: "hidden",
                textShadow: "0px 0px 3px black"
            },
            value : {
                color : "#333",
                letterSpacing: "normal",
                padding: "1px",
                width : "164px",
                verticalAlign: "middle",
                display: "inline-block"
            },
            flex : {
                paddingLeft: "3px"
                // display: "flex",
                // flexDirection: "row"
            },
            inputs: {
                border: "0px",
                borderRadius: "3px",
                marginLeft: "1px",
                width: "36px",
                height: "18px",
                float: "left"
                // flexGrow: 1
            }
        };
	}

	onChange(index) {
		return (ev) => {
			let val = Number(ev.target.value);
			this.state.values[index] = val;
			this.props.changeVecFunc(this.props.initialParam.name, index, val);
			this.setState({
				values : [].concat(this.state.values)
			});
		};
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
						key={key}
					></input>);
		});
		return values;
	}

	render () {
		const styles = this.styles.bind(this)();
		return (<div style={styles.view}>
					<div style={styles.key}>
						{this.props.initialParam.name}
					</div>
                    <div style={styles.value}>
                        <div style={styles.flex}>
                            {this.valueElem.bind(this)()}
                        </div>
                    </div>
				</div>);
	}
}
