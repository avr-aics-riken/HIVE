import React from "react";
import ReactDOM from "react-dom";
import Autosuggest from 'react-autosuggest';;
import Core from '../../Core';

export default class MenuNodeCreate extends React.Component {
    constructor(props) {
        super(props);

        this.state = {
            nodes : null,
            value: '',
            suggestions: this.getSuggestions('')
        };

        this.styles = this.styles.bind(this);
        this.floating = (this.props.focusFunction !== undefined);

        this.nodeChanged = this.nodeChanged.bind(this);

        // auto suggest
        this.onChange = this.onChange.bind(this);
        this.onSuggestionsUpdateRequested = this.onSuggestionsUpdateRequested.bind(this);
        this.getSuggestions = this.getSuggestions.bind(this);
        this.getSuggestionValue = this.getSuggestionValue.bind(this);
        this.renderSuggestion = this.renderSuggestion.bind(this);

        this.componentDidMount = this.componentDidMount.bind(this);
        this.componentWillUnmount = this.componentWillUnmount.bind(this);

        // 親に自分飛ばしてるところ
        if(this.props.focusFunction){
            this.props.focusFunction(this);
        }
    }

    componentDidMount(){
        this.props.store.on(Core.Constants.NODE_COUNT_CHANGED, this.nodeChanged);
    }

    componentWillUnmount(){
        this.props.store.removeListener(Core.Constants.NODE_COUNT_CHANGED, this.nodeChanged);
    }

    nodeChanged(err, data){
        this.setState({
            value: '',
            suggestions: this.getSuggestions.bind(this)('')
        });
    }

    // インプットボックスの change と自身のステートの更新はここ
    onChange(eve, {newValue}){
        this.setState({value: newValue});
    }

    // ここにある nodeNames にセットされた名前がサジェストされる
    getSuggestions(value) {
        var nodeNameList = this.props.store.getNodeNameList();
        var nodeNames = [];
        var flg = false;
        if(nodeNameList){
            for(let i in nodeNameList){
                nodeNames.push({name: nodeNameList[i]});
                if(nodeNameList[i] === value){flg = true;}
            }
        }

        // add node action
        if(flg){
            this.props.action.addNodeByName(value);
            if (this.floating && this.props.hiddenFunction !== undefined) {
                this.props.hiddenFunction();
            }
        }

        if(nodeNames.length === 0){return [];}
        const inputValue = value.trim().toLowerCase();
        const inputLength = inputValue.length;
        return inputLength === 0 ? [] : nodeNames.filter(nodename =>
             // nodename.name.toLowerCase().slice(0, inputLength) === inputValue
             nodename.name.toLowerCase().indexOf(inputValue) !== -1
        );
    }
    onSuggestionsUpdateRequested({value}){this.setState({suggestions: this.getSuggestions(value)});}
    getSuggestionValue(suggestion){
        return suggestion.name;
    }
    renderSuggestion(suggestion){return (<span>{suggestion.name}</span>);}

    styles() {
        return {
            block: {
                margin: "0px",
                padding: "0px",
                width: "100%"
            },
            suggestTheme: {
                // container:                   'react-autosuggest__container',
                // containerOpen:               'react-autosuggest__container--open',
                // input:                       'react-autosuggest__input',
                // suggestionsContainer:        'react-autosuggest__suggestions-container',
                // suggestion:                  'react-autosuggest__suggestion',
                // suggestionFocused:           'react-autosuggest__suggestion--focused',
                // sectionContainer:            'react-autosuggest__section-container',
                // sectionTitle:                'react-autosuggest__section-title',
                // sectionSuggestionsContainer: 'react-autosuggest__section-suggestions-container'
                input: {
                    backgroundColor: "#eee",
                    borderRadius: "3px",
                    border: "none",
                    color: "#666",
                    width: (this.props.floating) ? "234px" : "250px",
                    margin: "2px 3px",
                    padding: (this.props.floating) ? "5px 3px" : "2.5px 3px"
                },
                suggestionsContainer: {
                    listStyle: "none",
                    margin: "0px",
                    padding: "0px",
                    width: "100%",
                    position: "absolute"
                },
                suggestion: {
                    backgroundColor: "rgba(16, 16, 16, 0.9)",
                    borderLeft: "1px solid gray",
                    borderRight: "1px solid gray",
                    borderBottom: "1px solid gray",
                    color: "white",
                    fontSize: "small",
                    margin: "0px 0px 0px 2px",
                    padding: "5px",
                    width: (this.props.floating) ? "236px" : "251px",
                    overflow: "hidden"
                },
                suggestionFocused: {
                    backgroundColor: "rgba(16, 16, 16, 1.0)",
                    color: "rgb(2, 255, 245)"
                }
            }
        };
    }

    // 以下 placeholder に与えた文字列がインプットボックスの
    // なかに初期値として見えている文字列
    render(){
        const style = this.styles();
        const {value, suggestions} = this.state;
        const inputProps = {
            placeholder: 'Type a Node name.',
            value,
            onChange: this.onChange
        };
        return (
            <div style={style.block}>
                <Autosuggest theme={style.suggestTheme}
                    suggestions={suggestions}
                    onSuggestionsUpdateRequested={this.onSuggestionsUpdateRequested}
                    getSuggestionValue={this.getSuggestionValue}
                    renderSuggestion={this.renderSuggestion}
                    inputProps={inputProps}
                    ref="suggest"
                />
            </div>
        );
    }
}
