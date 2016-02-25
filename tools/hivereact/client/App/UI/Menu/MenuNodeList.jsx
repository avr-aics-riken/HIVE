import React from "react"
import ReactDOM from "react-dom"
import Core from '../../Core'

export default class MenuNodeList extends React.Component {
    constructor(props) {
        super(props);

        this.state = {
            nodes: null,
            nodeList: null
        };

        this.styles = this.styles.bind(this);
    }

    styles() {
        return {
            block: {
                backgroundColor: "rgba(0, 0, 0, 0.25)",
                margin: "0px",
                padding: "0px",
                width: "100%",
                height: "300px",
                boxShadow: "0px 0px 1px 0px white inset"
            },
            list: {
                lineHeight: "30px",
                width: "100%",
                height: "30px",
                boxShadow: "0px 0px 1px 0px white inset"
            }
        }
    }

    generator(value){
        const style = this.styles();
        let nodeNameList = this.props.store.getNodeNameList();
        if(nodeNameList && nodeNameList.length > 0){
            this.setState({nodeList: nodeNameList});
            return nodeNameList.map(list);
        }
        function list(val, key){
            return (
                <div style={style.list} key={key}>{this.state.nodeList[key]}</div>
            );
        }
    }

    render(){
        const style = this.styles();
        return (
            <div style={style.block}>
                {this.generator.bind(this)()}
            </div>
        );
    }
}
