<template>
  <div >
    <a class="button is-success is-rounded"
      @click="handleAdd"
      :disabled="this.inputArr.length > 10"
    >增加进程</a>
    <a class="button is-danger is-rounded"
      :disabled="this.inputArr.length <= 2"
      @click="handleDelete"
    >减少进程</a>
    <InputItem v-for="item in inputArr"
      :key="item.index"
      :name.sync="item.name"
      :value.sync="item.value"
      :index="item.index"
    />
  </div>
</template>

<script>
import InputItem from "./InputItem";

export default {
  components: {
    InputItem
  },
  data() {
    return {
      inputArr: [
        {
          index: 0,
          name: "test1",
          value: "3"
        },
        {
          index: 1,
          name: "test2",
          value: "4"
        }
      ]
    };
  },
  methods: {
    handleAdd() {
      this.inputArr.push({
        index: this.inputArr.length,
        name: `test${this.inputArr.length + 1}`,
        value: "2"
      });
    },
    handleDelete() {
      if (this.inputArr.length > 2) {
        this.inputArr.pop();
      }
    },
    getValues() {
      return this.inputArr;
    }
  }
};
</script>

<style scoped>
a {
  margin-bottom: 20px;
}
div {
  margin-bottom: 0px;
}
</style>